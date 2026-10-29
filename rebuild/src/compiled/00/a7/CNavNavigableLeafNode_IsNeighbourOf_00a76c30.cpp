// 00a76c30 CNavNavigableLeafNode::IsNeighbourOf
// Flags: /Gr
// Leaf: ?IsNeighbourOf@CNavNavigableLeafNode@@UBE_NPBVCANavQuadTreeNode@@@Z
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
class CANavQuadTreeNode: public CBaseClassNonCopyable {
public:
 virtual ~CANavQuadTreeNode();
 virtual bool IsNeighbourOf(const CANavQuadTreeNode*) const;
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
namespace std {
struct input_iterator_tag {};
struct forward_iterator_tag: input_iterator_tag {};
struct bidirectional_iterator_tag: forward_iterator_tag {};
struct random_access_iterator_tag: bidirectional_iterator_tag {};
// Fidelity: the original STLport _algobase.c random-access overload is visible.
template<class I,class T> __declspec(noinline) I __fastcall __find(I first,I last,const T&value,const random_access_iterator_tag&) {
 long count=(last-first)>>2;
 for(;count>0;--count) {
  if(*first==value)return first; ++first;
  if(*first==value)return first; ++first;
  if(*first==value)return first; ++first;
  if(*first==value)return first; ++first;
 }
 switch(last-first) {
 case 3: if(*first==value)return first; ++first;
 case 2: if(*first==value)return first; ++first;
 case 1: if(*first==value)return first; ++first;
 case 0: default:return last;
 }
}
}
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
bool CNavNavigableLeafNode::IsNeighbourOf(const CANavQuadTreeNode* node) const {
 return std::__find(Neighbours.begin(),Neighbours.end(),node,std::random_access_iterator_tag())!=Neighbours.end();
}