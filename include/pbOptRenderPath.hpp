
#include <vector>

template <class RenderNode, class RenderData>
struct PbRenderGraph
{  
   struct Data
   { 
	size_t _node;
	size_t  _me;
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

   bool computePath(vector<size_t> output, Path& path)
   {
	// first precompute all the needed information
	// --> used data
	vector<size_t> dataUsage(_data.size(),0u);
	vector<size_t> nodeUsage(_nodes.size(),0u);
	vector<size_t> nodeToProcess(_nodes.size(),_nodes.size());
	size_t currNodeToProcess = 0;
        size_t lastNodeToProcess = 0;

	// add output data usage and node to process
	for (auto outDataIdx : output)
	{ 
		dataUsage[outDataIdx]++;
		auto nodeIdx =  _data[outDataIdx]._node;
		if(nodeUsage[nodeIdx]==0)
		{
			nodeUsage[nodeIdx]=1;
			nodeToProcess[lastNodeToProcess]=nodeIdx;
			lastNodeToProcess++;
		}
	}
	// add node intermediate data
	while (nodeToProcess[currNodeToProcess]<_nodes.size())
	{
		auto nodeIdx = nodeToProcess[currNodeToProcess];
		for (auto inDataIdx : _nodes[nodeIdx]._inputData)
		{
			dataUsage[inDataIdx]++;
			auto inNodeIdx = _data[inDataIdx]._node;
			if (nodeUsage[inNodeIdx]==0)
			{
				nodeUsage[inNodeIdx]=1;
				nodeToProcess[lastNodeToProcess]=inNodeIdx;
				lastNodeToProcess++;
			}
		}
		++currNodeToProcess;
	}	
	
	// initialize the state
	const size_t maxPathLength = lastNodeToProcess;
        vector<bool> traversedNodes(_nodes.size(),false);
	float minMaxMemoryLoad = numeric_limits::max<float>();

	struct PathTraversalData
	{
	   size_t _nextStep;
	   float _currentMemoryLoad; 
	   float _maxMemoryLoad; 
	   vector<bool> _traversedNodes;

	   struct StepTraversalData
	   { 
		   Step _step;
		   float _maxMemoryLoad;
		   float _deltaMemoryLoad;
		   size_t _nodeIdx;
	   };
	   vector<StepTraversalData> _steps;

	   float getMaxMemoryLoad()
	   {
                   assert(_nextStep>0); 
		   return _steps[_nextStep-1] ._maxMemoryLoad;
	   }

	   void addStep(size_t nodeIdx)
	   {
		   assert(_nextStep<_steps.size());
                   _traversedNodes[nodeIdx] = true;
		   _steps[_nextStep]._nodeIdx = nodeIdx;
		   _steps[_nextStep]._maxMemoryLoad = nodeIdx;


		   _nextStep++;
	   }

	   void removeLastStep()
	   {
		   _nextStep--;
		   _currentMemoryLoad -= _steps[_nextStep]._deltaMemoryLoad;
		   _traversedNode[_steps[_nextStep]._nodeIdx] = false;
	   }

	   void getPathSteps(Path& path)
	   {
		   assert(path._steps.size()==_steps.size());
		   for (size_t sIdx=0; sIdx<=_nextStep;++sIdx)
		   {
			   path._steps[sIdx] = _steps[sIdx]._step;
		   }
	   }

	} currentPath;

	Path minMemoryLoadPath;


	size_t currPathLength;
	vector<size_t> pathPosNodes(maxPathLength, 0u);
	while (!done)
	{
		// all nodes for the current path position have been traversed
		if (pathPosNodes[currPathLength]>=maxPathLength) 
		{
			// all possible paths have been traversed
			if (currPathLength==0) break;
			// otherwise got to the previous path and visit the next node
			pathPosNodes[currPathLength]=0;
			currPathLength--;
			traversedNodes[pathPosNodes[currPathLength]] = false;
			pathPosNodes[currPathLength]++;
			continue;
		}
		// current node 
		const auto currNodeIdx = nodeToProcess[pathPosNodes[currPathLength]];
		// check if it's a valid path 
		// --> all needed nodes have been visited
		auto validNode = true;
		// TODO : replace input by nodes
		for (auto nodeInputIdx : _nodes[currNodeIdx].inputData)
		{
			if (!traversedNodes[_data[nodeInputIdx]._node])
			{
			   validNode = false;
			   break;
			}
		}
		if (!validNode) 
		{
	           pathPosNodes[currPathLength]++;
		   continue;
		}
		// create a step
		Step step; 
		step._node = currNodeIdx;
		step._memoryLoad = path._memoryLoad + getNodeMemoryLoad(currNodeIdx);
	        // check which inputs are consumed and update the path traversal data

		// check which outputs are produced and update the path traversal data	

	}
		


	
	
	// depth-first DAG traversal
	// browse the complete graph and evaluate each possible path 
        for (auto nodeStep)	
   }
}

class PbOptRenderPath
{
   public :
      
}

