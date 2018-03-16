/*! *****************************************************************************
 *   \file pbRenderGraph.h
 *   \author 2018
 *   \brief
 *   \date 2018-03-15
 *   *****************************************************************************/
#ifndef _INCLUDE_PBRENDERGRAPH_H
#define _INCLUDE_PBRENDERGRAPH_H

template <class RenderNode, class RenderData>
class PbRenderGraph final
{
   struct Data
   {
      size_t _node;
      size_t _me;
      RenderData _data;
   };
   vector<Data> _data;

   struct Node
   {
      size_t _me;
      size_t _outputDataStart;
      size_T _outputDataNb;
      size_t _firstParent;
      size_t _lastParent;
      vector<size_t> _inputData;
      RenderNode _node;
   };
   vector<Node> _nodes;

  public:
   enum class Error
   {
      SUCCESS,
      BAD_NODE,
      BAD_DATA
   };

   // Register a render node (data producer) in the graph
   // --> produced data are the data rendered by the given node : they must not be already
   // associated to any render node
   // --> consumed data are the data used by the node to render the produce its data : they must
   // been produced by an already registerd node
   Error registerNode(
       const RenderNode&,
       const std::vector<RenderData>& producedData,
       const std::vector<RenderData>& consumedData = std::vector<DataDescriptor>() ) &
       noexcept;
};

#endif  // _INCLUDE_PBRENDERGRAPH_H
