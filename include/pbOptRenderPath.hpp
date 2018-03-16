//*****************************************************************************/
//
// Filename pbOptRenderPath.hpp
//
//*****************************************************************************/

#ifndef _PBOPTRENDERPATH_H
#define _PBOPTRENDERPATH_H

#include <vector>

template <class RenderNode, class RenderData>
struct PbRenderGraph {
  struct Step {
    size_t _node;
    // net memory load of the node needed for computation ( does not include
    // consumed )
    float _computeMemoryLoad;
    float _producedMemory;  // net added memory to the path after computation
    vector<size_t> _produced;
  };
  vector<StepData> _stepsData;

  struct StepTraversalData {
    size_t _id;
    vector<size_t> _consumed;
    // consumed memory that could be released after computation
    float _consumedMemory;
    // max memory consumption of the current path up to this step
    float _maxMemoryLoad;
  };
  vector<StepTraversalData> _stepsState;

  PathTraversalData(const vector<size_t>& output)
      : _nSteps(0),
        _currStepId(0),
        _currentMemoryLoad(0),
        _traversedNodes(_nodes.size(), false),
        _dataRemainingConsumer(_data.size(), 0),
  {
    // --> set of nodes processed within the path
    vector<size_t> nodeToProcess(_nodes.size(), _nodes.size());

    size_t currNodeToProcess = 0;
    size_t lastNodeToProcess = 0;

    // add output data usage and the corrsponding nodes to process
    for (auto outDataIdx : output) {
      _dataRemainingConsumer[outDataIdx]++;
      auto nodeIdx = _data[outDataIdx]._node;
      // add the producing node if not already done
      if (!_traversedNodes[nodeIdx]) {
        _traversedNodes[nodeIdx] = true;
        nodeToProcess[lastNodeToProcess] = nodeIdx;
        lastNodeToProcess++;
      }
    }
    // add node intermediate data and the corresponding nodes to process
    while (nodeToProcess[currNodeToProcess] < _nodes.size()) {
      auto nodeIdx = nodeToProcess[currNodeToProcess];
      // add the node input data
      for (auto inDataIdx : _nodes[nodeIdx]._inputData) {
        _dataRemainingConsumer[inDataIdx]++;
        auto inNodeIdx = _data[inDataIdx]._node;
        // add the producing node if not already done
        if (!_traversedNodes[inNodeIdx]) {
          _traversedNodes[inNodeIdx] = true;
          nodeToProcess[lastNodeToProcess] = inNodeIdx;
          lastNodeToProcess++;
        }
      }
      ++currNodeToProcess;
    }

    _nSteps = lastNodeToProcess;

    // initialize the step data
    _stepsData.resize(_nSteps);
    for (size_t stepId = 0; stepId < _nSteps; ++stepId) {
      StepData& stepData = _stepsData[stepId];
      const size_t nodeId = stepData._node = nodeToProcess[stepId];
      _traversedNodes[nodeId] = false;
      stepData._computeMemoryLoad =
          0;  //_nodes[nodeId]._node.getMaxMemoryLoad();
      stepData._producedMemory = 0;
      stepData._produced.reserve(_nodes[nodeId]._outputDataNb);
      for (size_t nodeDataId = 0; nodeDataId < _nodes[nodeId]._outputDataNb;
           ++nodeDataId) {
        const auto dataId = nodeDataId + _nodes[nodeId]._outputDataStart;
        const float memory = _data[dataId]._data.size();
        stepData._computeMemoryLoad += memory;
        if (_dataRemainingConsumer[dataId] > 0) {
          _produced.push_back(dataId);
          stepData._producedMemory += memory;
        }
      }
    }

    // initialize state
    _stepsState.resize(_nSteps);
    for (size_t stepId = 0; stepId < _nSteps; ++stepId) {
      _stepsState[stepId]._id = _nSteps;
    }
  }

  float getMaxMemoryLoad() {
    return _currStepId == _nSteps ? _stepsState.back()._maxMemoryLoad
                                  : std::numeric_limits::max<float>();
  }

  // set the traversal to the next possible path
  bool setNextPath() {
    // we are at the end of a path -> backtrack untill we find a valid step
    while (!setNextNodeToStep() && gotoPreviousStep())
      ;

    // we are at an unvisited step -> fortrack to the end of the path
    while (gotoNextStep() && setNextNodeToStep())
      ;

    return _currStepId == _nSteps;
  }

  // find the next possible node at the current path step (depth)
  // --> return false if all possible node at the current step have been
  // evaluated
  // --> return true if a new node has been set (step data has been updated
  // accordingly)
  bool setNextNodeToStep() {
    // reset the current node
    resetNodeToStep();
    // set the next one
    size_t currId = (_stepsState[_currStepId]._id + 1) % _nSteps;
    while ((currId < _nSteps) && !setNodeToStep(currId)) {
      currId++;
    }
    return currId < _nSteps;
  }

  // reset the current node at the current step
  void resetNodeInStep() {
    const size_t nodeId = _stepsState[_currStepId]._id;
    if (nodeId < _nSteps) {
      // remove the node from the traversed nodes
      _traversedNodes[_stepsData[nodeId]._node] = false;
      // get the step
      const Step& step = _stepsData[nodeId]._step;
      // update the node input data usage
      for (auto dataId : _nodes[step._node]._inputData)
        _dataRemainingConsumer[dataId]++;
      // update the node output data
      for (auto dataId : step.produced) _dataProduced[dataId]--;
      // update the current memory load
      _currentMemoryLoad +=
          stepsState[stepId]._consumedMemory - step._producedMemory;
      // reset the node
      _stepsState[_stepId]._id = _nSteps;
    }
  }

  bool setNodeToStep(const size_t nodeId) {
    // check node id validity
    if (nodeId >= _nSteps) return false;
    // check if the current node has node been already processed in the
    // current path
    if (_traversedNodes[_stepsData[nodeId]._node]) return false;
    // get the node idx
    const auto nodeIdx = _stepsData[nodeId]._step._node;
    // check if the current node inputs have been produced in the current
    // path
    for (auto inDataId : _nodes[nodeIdx]._inputData)
      if (!_traversedNodesdataProduced[inDataId]) return false;
    // d
    StepTraversalData& stepSate = _stepsState[_stepId];
    stepState._id = nodeId;
    for (auto dataId : _stepsData[nodeId]._produced) {
      _dataProduced[dataId]++;
    }
    stepState._consumedMemory = 0;
    for (auto dataId : _nodes[nodeIdx]._inputData) {
      if (--_dataRemainingConsumer[dataId] == 0) {
        stepState._consumedMemory += _data[dataId].size();
      };
    }
    // update the path memory load
    const float prevMaxMemoryLoad =
        _stepId == 0 ? 0.0f : _stepsData[_stepId - 1]._maxMemoryLoad;
    const float stepMemoryLoad =
        _currentMemoryLoad + _stepsData[nodeId]._computeMemoryLoad;
    _currentMemoryLoad +=
        _stepsData[nodeId]._producedMemory - stepState._consumedMemory;
    stepState._maxMemoryLoad = std::max(stepMemoryLoad, prevMaxMemoryLoad);
  }

  // set the current step to the next one
  bool gotoNextStep(size_t nodeIdx) { return ++_currStepId >= _nSteps; }

  // set the current step to the previous one
  void gotoPreviousStep() { return --_currStepId > 0; }

  // assuming we are at the endo a path : copy the path data to the given
  // one
  void getSteps(vector<Step>& steps) {
    if (_currStepId != _nSteps) {
      steps.clear();
      return;
    }
    steps.resize(_nSteps);
    for (size_t stepId = 0; setpId < _nSteps; ++stepId) {
      const StepTraversalData& stepState = _stepsState[stepId];
      const StepData& stepData = _stepsData[stepState._id];
      steps[stepId]._node = stepData._node;
      steps[stepId]._produced = stepData._produced;
      steps[stepId]._consumed = stepState._consumed;
    }
  }
};

float computePath(Path& path) {
  PathTraversalData currPath(path._output);
  const float minPathMemoryLoad = std::numeric_limits::max<float>();

  // depth first search of every potential path for the one with the minimum
  // maximum memory usage
  while (currPath.setNextPath()) {
    // the path is the new optimal : update
    if (currPath.getMaxMemoryLoad() < minPathMemoryLoad) {
      minPathMemoryLoad = currPath.getMaxMemoryLoad();
      currPath.getSteps(path._steps);
    }
  }
  return minPathMemoryLoad;
}
}

#endif  // _PBOPTRENDERPATH_H
