// 00a77420 CNavSwitchableLeafNode::Unlock
// Flags: /Gr
// Leaf: ?Unlock@CNavSwitchableLeafNode@@AAEXPBVCNavRegion@@@Z
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end().
#include "fable_compiler.h"
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
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0
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
 FABLE_FORCEINLINE const T* begin() const {return Start;}
 FABLE_FORCEINLINE const T* end() const {return Start+Size;}
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
#include "engine/CVectorMap.h"
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