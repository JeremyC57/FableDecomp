#ifndef FABLE_ENGINE_CPRIORITYQUEUE_H
#define FABLE_ENGINE_CPRIORITYQUEUE_H
#include "stlport/fable_containers.h"
// Layout: both original PDB instantiations have one private vector Heap; TLC vector is12 bytes.
template<class T>class CPriorityQueue { private: std::vector<T> Heap; };
#endif
