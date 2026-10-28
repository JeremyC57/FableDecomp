// CEngineLandscapeMemoryManager::DeleteIndexBufferBlockWhileBusy(buffer) -- queue a nonnull resource in the current delayed-disposal list.
// Leaf: ?DeleteIndexBufferBlockWhileBusy@CEngineLandscapeMemoryManager@@AAEXPAVCIndexBuffer@@@Z
// Identity: proven; both PDB exact original private DeleteIndexBufferBlockWhileBusy(CIndexBuffer*) API; own CurrentDisposalListIndex+34 selects three STLport list sentinel pointers at+28; source fields agree shared header and donor names; original multiple bases CFailedAllocationHandler@0/CMovableResource@4 proven RTTI; malloc00BFEA0E exactimport .
// Layout: shared manager header retained unchanged under evidence alias; real original two polymorphic bases and typed std::list<T*>[3] members restore flattened data. Genuine STLport _List_base, class iterator with original _Nonconst_traits and copy constructor, protected _M_insert(iterator,constT&), node allocation, placement construction and tail insertion. No resource fields/sizeof used. CMovableResource Verify and CanRelocateData APIs declared; header offers only flattened vptr.
#define CEngineLandscapeMemoryManager CEngineLandscapeMemoryManager_HeaderFields
#include "engine/CEngineLandscapeMemoryManager.h"
#undef CEngineLandscapeMemoryManager
#ifndef _MSC_VER
#define __fastcall
#define __cdecl
#define __forceinline inline
typedef __SIZE_TYPE__ PlacementSize;
#else
typedef unsigned int PlacementSize;
#endif
inline void* operator new(PlacementSize,void* p) { return p; }
#ifdef _MSC_VER
extern "C" void* __cdecl malloc(unsigned int);
#else
#include <stdlib.h>
#endif
namespace std{template<class T>class allocator{};struct _List_node_base{_List_node_base* _M_next;_List_node_base* _M_prev;};template<class T>struct _List_node:public _List_node_base{T _M_data;};template<class T>struct _Nonconst_traits{typedef T& reference;};struct _List_iterator_base{_List_node_base* _M_node;_List_iterator_base(_List_node_base* p):_M_node(p){}};template<class T,class Traits>struct _List_iterator:public _List_iterator_base{_List_iterator(_List_node_base* p):_List_iterator_base(p){}_List_iterator(const _List_iterator& p):_List_iterator_base(p._M_node){}};template<class T,class Al>class _List_base{public:_List_node_base* _M_node;};template<class T,class Al=allocator<T> >class list:public _List_base<T,Al>{public:typedef _List_iterator<T,_Nonconst_traits<T> > iterator;__forceinline iterator end(){return iterator(this->_M_node);}__forceinline void push_back(const T& value){_M_insert(end(),value);}protected:__forceinline void _M_insert(iterator position,const T& value){_List_node<T>* node=(_List_node<T>*)malloc(sizeof(_List_node<T>));new(&node->_M_data)T(value);_List_node_base* previous=position._M_node->_M_prev;node->_M_next=position._M_node;node->_M_prev=previous;previous->_M_next=node;position._M_node->_M_prev=node;}};}
class CFailedAllocationHandler{public:virtual ~CFailedAllocationHandler(){} };class CMovableResource{public:virtual ~CMovableResource(){}virtual unsigned long Verify()const;virtual bool CanRelocateData(long)const;};class CMovableResourceMemoryPool;class CXboxBufferMemoryPool;class CTexture;class CVertexBufferWin32;class CIndexBuffer;
class CEngineLandscapeMemoryManager:public CFailedAllocationHandler,public CMovableResource{public:CMovableResourceMemoryPool* VirtualMemPool;CXboxBufferMemoryPool* PhysicalMemPool;std::list<CTexture*> TextureDisposalList[3];std::list<CVertexBufferWin32*> VertexBufferDisposalList[3];std::list<CIndexBuffer*> IndexBufferDisposalList[3];long CurrentDisposalListIndex;bool InsideDefragment,PoolAllocationEnabled;long AllocationCount;bool EnableDelayedResourceDestruction;private:void DeleteIndexBufferBlockWhileBusy(CIndexBuffer*);friend struct TestAccess;};void CEngineLandscapeMemoryManager::DeleteIndexBufferBlockWhileBusy(CIndexBuffer* buffer){if(buffer)IndexBufferDisposalList[CurrentDisposalListIndex].push_back(buffer);}