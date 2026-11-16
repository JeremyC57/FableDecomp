#ifndef FABLE_ENGINE_CPRIORITYQUEUE_H
#define FABLE_ENGINE_CPRIORITYQUEUE_H
#include "stlport/fable_containers.h"
// Layout: both original PDB instantiations have one private vector Heap; TLC vector is12 bytes.
#ifndef FABLE_RETAIL_CPRIORITYQUEUE_API_PASS23
template<class T>class CPriorityQueue { private: std::vector<T> Heap; };
#else
template<class T>class CPriorityQueue { private: std::vector<T> Heap;
#ifdef FABLE_RETAIL_CPRIORITYQUEUE_API_PASS23
public:
 bool IsEmpty() const {return Heap.empty();}
 unsigned long Size() const {return Heap.size();}
 void Clear() {Heap.clear();}
 // Native Initialise/CalculateRoute call original PushA22460; budget ladder cannot retain this call while expanding state copies.
 template<class C>
#ifdef FABLE_RETAIL_CPRIORITYQUEUE_PUSH_CALL_PASS23
 FABLE_STLP_NOINLINE
#endif
 void Push(const T& element,const C& compare) {
  Heap.push_back(element);
#ifdef FABLE_RETAIL_CPRIORITYQUEUE_PUSH_RANGE_PASS23
  std::push_heap(Heap.begin(),Heap.end(),compare);
#else
  std::__push_heap(Heap.begin(),ptrdiff_t(Heap.size()-1),ptrdiff_t(0),T(Heap.back()),compare);
#endif
 }
 template<class C> T Pop(const C& compare) {
  T result=Heap.front();
#ifdef FABLE_RETAIL_CPRIORITYQUEUE_POP_HELPER_PASS23
  std::__pop_heap(Heap.begin(),Heap.end()-1,Heap.end()-1,T(*(Heap.end()-1)),compare,(ptrdiff_t*)0);
#else
  T value=Heap.back();Heap.back()=Heap.front();
  std::__adjust_heap(Heap.begin(),ptrdiff_t(0),ptrdiff_t(Heap.size()-1),value,compare);
#endif
  Heap.pop_back();return result;
 }
#ifdef FABLE_RETAIL_CPRIORITYQUEUE_PUSH_RANGE_PASS23
 template<class C> void UpdateElement(const T& element,const C& compare) {
  for(typename std::vector<T>::iterator i=Heap.begin();i!=Heap.end();++i)
#ifdef FABLE_RETAIL_CPRIORITYQUEUE_ELEMENT_FIRST_PASS23
   if(element==*i){std::push_heap(Heap.begin(),i+1,compare);return;}
#else
   if(*i==element){std::push_heap(Heap.begin(),i+1,compare);return;}
#endif
 }
#else
 template<class C> void UpdateElement(const T& element,const C& compare) {
  for(typename std::vector<T>::iterator i=Heap.begin();i!=Heap.end();++i)
   if(*i==element){std::__push_heap(Heap.begin(),ptrdiff_t(i-Heap.begin()),ptrdiff_t(0),T(*i),compare);return;}
 }
#endif
#endif
};
#endif
#endif
