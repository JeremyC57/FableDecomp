// 00a77420 CNavSwitchableLeafNode::Unlock
// Flags: /Gr
// Leaf: ?Unlock@CNavSwitchableLeafNode@@AAEXPBVCNavRegion@@@Z
#ifndef _MSC_VER
#include <stdint.h>
#endif
#define CANavQuadTreeNode Pass9NodeFields
#include "engine/CANavQuadTreeNode.h"
#undef CANavQuadTreeNode
#define CNavNavigableLeafNode Pass9LeafFields
#include "engine/CNavNavigableLeafNode.h"
#undef CNavNavigableLeafNode
#ifndef _MSC_VER
#define __fastcall
#define __forceinline inline
#define __declspec(x)
#endif
class CBaseClass { public: virtual ~CBaseClass(); };
class CBaseClassNonCopyable: public CBaseClass { protected: virtual ~CBaseClassNonCopyable(); };
class C2DVector { public: float X,Y; };
template<class T,int N> class CSmallVector;
enum ENavQuadTreeNodeType {};
class CANavQuadTreeNode: public CBaseClassNonCopyable {
public:
 virtual ~CANavQuadTreeNode();
 virtual CSmallVector<const CANavQuadTreeNode*,8>* GetNeighbours();
 virtual const CSmallVector<const CANavQuadTreeNode*,8>* GetNeighbours() const;
 virtual bool HasNeighbours() const;
 virtual bool IsNeighbourOf(const CANavQuadTreeNode*) const;
 virtual long GetRegionID() const;
 virtual ENavQuadTreeNodeType GetNodeType() const;
 C2DVector Position;
 unsigned char QuadTreeLevel,NavLayer;
 bool LeafNode,Blocked;
};
// Layout: donor CSmallVector Start+0/Size+4; native sign-extends Size.
template<class T,int N> class CSmallVector {
public:
 T* Start; signed char Size;
 __forceinline const T* begin() const {return Start;}
 __forceinline const T* end() const {return Start+Size;}
};
class CNavRegion;
// Layout: real RTTI base CANavQuadTreeNode+0, PRegion+16, Neighbours+20,
// PreferabilityCost+28. Header projection above retains donor/retail evidence.
class CNavNavigableLeafNode: public CANavQuadTreeNode {
public:
 virtual bool IsNeighbourOf(const CANavQuadTreeNode* node) const;
 CNavRegion* PRegion;
 CSmallVector<const CANavQuadTreeNode*,8> Neighbours;
 unsigned char PreferabilityCost;
};
FABLE_STATIC_ASSERT(sizeof(CANavQuadTreeNode)==sizeof(Pass9NodeFields));
FABLE_STATIC_ASSERT(sizeof(CNavNavigableLeafNode)==sizeof(Pass9LeafFields));

#define CNavSwitchableLeafNode Pass9SwitchableFields
#include "engine/CNavSwitchableLeafNode.h"
#undef CNavSwitchableLeafNode
// Layout: same real base at0; original UniqueIDsOpen map at32 occupies16 retail bytes.
// Original typed CVectorMap/CArray/STLport vector hierarchy restores the container.
#ifdef _MSC_VER
typedef unsigned __int64 Pass9Key;
#else
typedef uint64_t Pass9Key;
#endif
namespace std {
template<class A,class B>struct pair{A first;B second;};
template<class T>class allocator{};
template<class V,class T,class A>class _STLP_alloc_proxy:public A{public:V _M_data;};
template<class T,class A>class _Vector_base{protected:T*_M_start;T*_M_finish;_STLP_alloc_proxy<T*,T,A>_M_end_of_storage;};
template<class T,class A=allocator<T> >class vector:public _Vector_base<T,A>{public:
 typedef unsigned int size_type;
 const T*begin()const{return this->_M_start;}
 size_type size()const{return size_type(this->_M_finish-this->_M_start);}
 const T&operator[](size_type n)const{return *(begin()+n);}
};
}
template<class T>class CArray:public std::vector<T>{public:const T&operator[](unsigned int n)const{return *(this->begin()+n);}};
template<class K,class V>class CKeyPairCompareLess{};
template<class K,class V,class C=CKeyPairCompareLess<K,V> >class CVectorMap:public CArray<std::pair<K,V> >{public:C Compare;bool Dirty;};
class CNavSwitchableLeafNode:public CNavNavigableLeafNode {
 CVectorMap<Pass9Key,bool> UniqueIDsOpen;
 bool IsEveryUniqueIDOpen() const;
 void Unlock(const CNavRegion* region);
};
FABLE_STATIC_ASSERT(sizeof(CNavSwitchableLeafNode)==sizeof(Pass9SwitchableFields));
// Layout: original CVectorMap array base has start+0,finish+4.
// Pair<unsigned64,bool> has its original second at8 and stride16.
#ifdef _MSC_VER
#pragma auto_inline(off)
#endif
bool CNavSwitchableLeafNode::IsEveryUniqueIDOpen()const {
 for(unsigned int i=0;i<UniqueIDsOpen.size();++i)if(!UniqueIDsOpen[i].second)return false;
 return true;
}
#ifdef _MSC_VER
#pragma auto_inline(on)
#endif
void CNavSwitchableLeafNode::Unlock(const CNavRegion* region) {
 Blocked=false;
 PRegion=const_cast<CNavRegion*>(region);
 const CSmallVector<const CANavQuadTreeNode*,8>* neighbours=static_cast<const CNavSwitchableLeafNode*>(this)->GetNeighbours();
 for(const CANavQuadTreeNode*const* it=neighbours->begin();it!=neighbours->end();++it) {
  CNavSwitchableLeafNode* node=const_cast<CNavSwitchableLeafNode*>(static_cast<const CNavSwitchableLeafNode*>(*it));
  if(node->GetNodeType()==(ENavQuadTreeNodeType)3 && node->Blocked && node->IsEveryUniqueIDOpen())node->Unlock(region);
 }
}