#include <stdio.h>
#include <string.h>
#include <stdlib.h>
// 00a76c70 CNavSwitchableLeafNode::IsNeighbourOf
// Flags: /Gr
// Leaf: ?IsNeighbourOf@CNavSwitchableLeafNode@@UBE_NPBVCANavQuadTreeNode@@@Z
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

#define CNavSwitchableLeafNode Pass9SwitchableFields
#include "engine/CNavSwitchableLeafNode.h"
#undef CNavSwitchableLeafNode
// Layout: RTTI gives CNavNavigableLeafNode at zero; header retail size48.
// Unread trailing map storage is represented only by its measured size.
class CNavSwitchableLeafNode:public CNavNavigableLeafNode{
 unsigned char pass9_unread_tail[16];
public:virtual bool IsNeighbourOf(const CANavQuadTreeNode* node)const;
};
FABLE_STATIC_ASSERT(sizeof(CNavSwitchableLeafNode)==sizeof(Pass9SwitchableFields));
bool CNavSwitchableLeafNode::IsNeighbourOf(const CANavQuadTreeNode* node)const{
 if(!Blocked)return CNavNavigableLeafNode::IsNeighbourOf(node);
 return false;
}

CBaseClass::~CBaseClass() {}
CBaseClassNonCopyable::~CBaseClassNonCopyable() {}
CANavQuadTreeNode::~CANavQuadTreeNode() {}
bool CANavQuadTreeNode::IsNeighbourOf(const CANavQuadTreeNode*)const{return false;}
static void check(bool x){if(!x)abort();}
int main(){
 CANavQuadTreeNode pool[130]; CNavSwitchableLeafNode subject;
 const CANavQuadTreeNode* storage[130]; unsigned char before[sizeof(subject)];
 subject.Position.X=2.0f;subject.Position.Y=-4.0f;subject.QuadTreeLevel=7;subject.NavLayer=3;
 subject.LeafNode=true;subject.Blocked=false;subject.PRegion=0;subject.PreferabilityCost=37;
 for(int n=0;n<=127;++n){
  for(int mode=0;mode<3;++mode){
   storage[0]=&pool[129];storage[129]=&pool[128];
   for(int i=0;i<n;++i)storage[i+1]=(mode==0?&pool[i]:(mode==1?&pool[i%9]:(i%3==0?0:&pool[i])));
   subject.Neighbours.Start=storage+1;subject.Neighbours.Size=(signed char)n;
   for(int needle=-1;needle<130;++needle){
    const CANavQuadTreeNode* wanted=needle<0?0:&pool[needle];
    bool expected=false;for(int k=0;k<n;++k)if(storage[k+1]==wanted)expected=true;
    const CANavQuadTreeNode* saved[130];memcpy(saved,storage,sizeof(storage));
    memcpy(before,&subject,sizeof(subject));
    check(subject.CNavSwitchableLeafNode::IsNeighbourOf(wanted)==expected);
    subject.Blocked=true;check(!subject.CNavSwitchableLeafNode::IsNeighbourOf(wanted));subject.Blocked=false;
    check(memcmp(before,&subject,sizeof(subject))==0);check(memcmp(saved,storage,sizeof(storage))==0);
   }
  }
 }
 puts("00a76c70_TEST PASS");return 0;
}