
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
       vector<size_t> _outputData;
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
	  vector<int> _dataRemainingConsumer;
	  vector<int> _dataProduced; 
   };

   bool computePath(vector<size_t> output, Path& path)
   {
	// first precompute all the needed information
        // --> number of consumer per data
	// --> active nodes for the requested output	
	// initialize the state
	
	
	
	// browse the complete graph and evaluate each possible path 
        	
   }
}

class PbOptRenderPath
{
   public :
      
}

