
//*****************************************************************************/
//
// Filename pbOptRenderPath.hpp
//
//*****************************************************************************/

#ifndef _PBOPTRENDERPATH_H
#define _PBOPTRENDERPATH_H

#include <vector>

template <class RenderNode, class RenderData>
struct PbRenderGraph
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
      vector<size_t> _inputData;
      size_t _firstParent;
      size_t _lastParent;
   };
   vector<Node> _nodes;

   struct Step
   {
      size_t _node;
      vector<size_t> _produced;
      vector<size_t> _consumed;
   };

   struct Path
   {
      vector<size_t> _output;
      vector<Step> _steps;
   };

   struct PathState
   {
      // log the data to be consumed == data usage - consumed data by the path
      // used to decide when a data could be disposed
      vector<int> _dataRemainingConsumer;
      // the currently produced data
      // used to decide if a node could be rendered (its input have been produced)
      vector<int> _dataProduced;
   };

   struct PathTraversalData
   {
      const size_t _nNodes;
      size_t _nextStep;
      float _currentMemoryLoad;
      vector<bool> _traversedNodes;

      struct StepTraversalData
      {
         Step _step;
         float _maxMemoryLoad;
         float _deltaMemoryLoad;
         size_t _nodeIdx;
      };
      vector<StepTraversalData> _steps;

      PathTraversalData( size_t nSteps )
          : _nextStep( 0 ),
            _currentMemoryLoad( 0 ),
            _traversedNodes( nSteps, false ),
            _steps( nSteps )
      {
      }

      float getMaxMemoryLoad()
      {
         return _nextStep > 0 ? _steps[_nextStep - 1]._maxMemoryLoad : .0f;
      }

      bool setNextPath()
      {
         // we are at the end of a path -> backtrack till we find a valid step
         while ( !setNextNodeToStep() && gotoPreviousStep() )
            ;

         // we are at an unvisited step -> fortrack to the end of the path
         while ( gotoNextStep() && setNextNodeToStep() )
            ;
      }

      bool setNextNodeToStep() { while }

      void gotoNextStep( size_t nodeIdx )
      {
         if ( _nextStep == _steps.size() ) return false;
         const float prevMaxMemoryLoad = getMaxMemoryLoad();
         _steps[_nextStep]._nodeIdx = nodeIdx;
         float totalMemoryLoad;
         _steps[_nextStep]._maxMemoryLoad = std::max( prevMaxMemoryLoad, totalMemoryLoad );
         _nextStep++;
         return true;
      }

      void gotoPreviousStep()
      {
         if ( _nextStep == 0 ) return false;
         _nextStep--;
         _currentMemoryLoad -= _steps[_nextStep]._deltaMemoryLoad;
         _traversedNode[_steps[_nextStep]._nodeIdx] = false;
         return true;
      }

      void getSteps( Path& path )
      {
         assert( path._steps.size() == _steps.size() );
         for ( size_t sIdx = 0; sIdx <= _nextStep; ++sIdx )
         {
            path._steps[sIdx] = _steps[sIdx]._step;
         }
      }

      bool terminated() const { return _nextStep == _steps.size(); }
   };

   bool computePath( vector<size_t> output, Path& path )
   {
      // first precompute all the needed information
      // --> used data
      vector<size_t> dataUsage( _data.size(), 0u );
      vector<size_t> nodeUsage( _nodes.size(), 0u );
      vector<size_t> nodeToProcess( _nodes.size(), _nodes.size() );
      size_t currNodeToProcess = 0;
      size_t lastNodeToProcess = 0;

      // add output data usage and node to process
      for ( auto outDataIdx : output )
      {
         dataUsage[outDataIdx]++;
         auto nodeIdx = _data[outDataIdx]._node;
         if ( nodeUsage[nodeIdx] == 0 )
         {
            nodeUsage[nodeIdx] = 1;
            nodeToProcess[lastNodeToProcess] = nodeIdx;
            lastNodeToProcess++;
         }
      }
      // add node intermediate data
      while ( nodeToProcess[currNodeToProcess] < _nodes.size() )
      {
         auto nodeIdx = nodeToProcess[currNodeToProcess];
         for ( auto inDataIdx : _nodes[nodeIdx]._inputData )
         {
            dataUsage[inDataIdx]++;
            auto inNodeIdx = _data[inDataIdx]._node;
            if ( nodeUsage[inNodeIdx] == 0 )
            {
               nodeUsage[inNodeIdx] = 1;
               nodeToProcess[lastNodeToProcess] = inNodeIdx;
               lastNodeToProcess++;
            }
         }
         ++currNodeToProcess;
      }

      // initialize the state
      const size_t maxPathLength = lastNodeToProcess;
      PathTraversalData currPath(  );
      const float minPathMemoryLoad = std::numeric_limits::max<float>();

      while ( currPath.setNextPath() )
      {
         if ( currPath.getMaxMemoryLoad() < minPathMemoryLoad )
         {
            minPathMemoryLoad = currPath.getMaxMemoryLoad();
            currPath.getSteps( path );
         }
      }
   }
}

#endif  // _PBOPTRENDERPATH_H
