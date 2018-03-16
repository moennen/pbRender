/*! *****************************************************************************
 *   \file pbRenderGraph.C
 *   \author 2018
 *   \brief
 *   \date 2018-03-15
 *   *****************************************************************************/

#include <include/pbRenderGraph.h>

#include <algorithm>

//----------------------------------------------------------
//
template <class RenderNode, class RenderData>
    PbRenderGraph<RenderNode, RenderData>::Error
    PbRenderGraph<RenderNode, RenderData>::registerNode(
        const RenderNode& nodeDesc,
        const std::vector<RenderData>& producedData,
        const std::vector<RenderData>& consumedData = std::vector<DataDescriptor>() ) &
    noexcept
{
   // check that the given node is not already registered
   auto compareRenderNode = [const &nodeDesc](const Node& node){return node._node == nodeDesc};
   if (std::find(_nodes.begin(), _nodes.end(), compareRenderNode) != _nodes.end() ) return Error::BAD_NODE;

   // check that every consummed data are produced by a registered node
   // and compute the closest / farthest parent node
   size_t closestParent = std::numeric_limits<size_t>::max();
   size_t farthestParent = 0u;
   std::vector<size_t> consumedDataIds;
   consumedDataIds.reserve(consumedData.size());
   for ( auto dataDesc : consumedData )
   {
      auto compareRenderData = [const &dataDesc](const Data& data){return data._data == dataDesc};
      auto dataIt = std::find(_data.begin(), _data.end(), compareRenderData);
      if (dataIt == _data.end()) return Error::BAD_DATA;
      closestParent = std::min( closestParent, dataIt->_node );
      farthestParent = std::max( farthestParent, dataIt->_node );
      consumedDataIds.push_back(dataIt->_me);
   }

   // Create the node
   _nodes.push_back( {_nodes.size(),
                      _data.size(),
                      producedData.size(),
                      farthestParent,
                      closestParent,
                      std::move( consumedDataIds ),
                      nodeDesc} );

   // Create the data
   _data.reserve( _data.size() + producedData.size() );
   for ( auto data : producedData )
   {
      _data.push_back( {_data.size(), _nodes.back()._me, data} );
   }

   return Error::SUCCESS;
}
