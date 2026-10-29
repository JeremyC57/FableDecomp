#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifndef _MSC_VER
#include <stdint.h>
typedef uint64_t Pass9Word64;
#define uint64_t Pass9Word64
#endif
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
 friend class Pass9UnlockFixture;
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

#ifdef _MSC_VER
typedef unsigned __int64 TestKey;
#else
typedef Pass9Word64 TestKey;
#endif
typedef std::pair<TestKey,bool> TestEntry;
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(sizeof(TestEntry)==16);
#else
FABLE_STATIC_ASSERT(sizeof(TestEntry)==12);
#endif
static CANavQuadTreeNode* testNodes[5];
static int testTypes[5];
CBaseClass::~CBaseClass(){}
CBaseClassNonCopyable::~CBaseClassNonCopyable(){}
CANavQuadTreeNode::~CANavQuadTreeNode(){}
CSmallVector<const CANavQuadTreeNode*,8>* CANavQuadTreeNode::GetNeighbours(){return &static_cast<CNavNavigableLeafNode*>(this)->Neighbours;}
const CSmallVector<const CANavQuadTreeNode*,8>* CANavQuadTreeNode::GetNeighbours()const{return &static_cast<const CNavNavigableLeafNode*>(this)->Neighbours;}
bool CANavQuadTreeNode::HasNeighbours()const{return true;}
bool CANavQuadTreeNode::IsNeighbourOf(const CANavQuadTreeNode*)const{return false;}
bool CNavNavigableLeafNode::IsNeighbourOf(const CANavQuadTreeNode*)const{return false;}
long CANavQuadTreeNode::GetRegionID()const{return 0;}
ENavQuadTreeNodeType CANavQuadTreeNode::GetNodeType()const{for(int i=0;i<5;++i)if(testNodes[i]==this)return (ENavQuadTreeNodeType)testTypes[i];abort();return (ENavQuadTreeNodeType)0;}
class Pass9UnlockFixture{
public:static void setup(CNavSwitchableLeafNode&n,TestEntry*p,int count){TestEntry* bounds[3]={p,p+count,p+3};memcpy(&n.UniqueIDsOpen,bounds,12);}
 static void run(CNavSwitchableLeafNode&n,const CNavRegion*r){n.Unlock(r);}
};
static void check(bool x){if(!x)abort();}
int main(){
 CNavSwitchableLeafNode nodes[5]; TestEntry flags[5][3]; const CANavQuadTreeNode* links[5][5];
 unsigned int seed=19;int expected[5];bool initial[5];unsigned char regionStorage[8];CNavRegion* region=reinterpret_cast<CNavRegion*>(regionStorage);
 for(int trial=0;trial<4000;++trial){
  for(int i=0;i<5;++i){
   testNodes[i]=&nodes[i];testTypes[i]=(trial%7==i?2:3);
   seed=seed*1664525u+1013904223u;initial[i]=(seed&256)!=0;
   nodes[i].Blocked=initial[i];nodes[i].PRegion=0;nodes[i].Position.X=2.0f;nodes[i].Position.Y=-4.0f;nodes[i].PreferabilityCost=12;
   for(int j=0;j<3;++j){flags[i][j].first=j;flags[i][j].second=((seed>>(j+2))&1)!=0;}
   Pass9UnlockFixture::setup(nodes[i],flags[i],trial%4);
   int count=0;for(int j=0;j<5;++j)if((seed>>(j+8))&1)links[i][count++]=&nodes[j];
   nodes[i].Neighbours.Start=links[i];nodes[i].Neighbours.Size=(signed char)count;expected[i]=0;
  }
  expected[0]=1;bool changed=true;
  while(changed){changed=false;for(int i=0;i<5;++i)if(expected[i])for(int k=0;k<nodes[i].Neighbours.Size;++k){int j=0;while(testNodes[j]!=links[i][k])++j;bool all=true;for(int z=0;z<trial%4;++z)all=all&&flags[j][z].second;if(!expected[j]&&initial[j]&&testTypes[j]==3&&all){expected[j]=1;changed=true;}}}
  Pass9UnlockFixture::run(nodes[0],region);
  for(int i=0;i<5;++i){check(nodes[i].Blocked==(expected[i]?false:initial[i]));check(nodes[i].PRegion==(expected[i]?region:0));check(nodes[i].Position.X==2.0f&&nodes[i].Position.Y==-4.0f&&nodes[i].PreferabilityCost==12);}
 }
 puts("00a77420_TEST PASS");return 0;
}