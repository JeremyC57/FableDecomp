// CAIStateGroup_CreatureUseCoverBase::GetHiddenCoverPosAndSetUpCoverThing(C3DVector&) -- find a live available cover, release old cover, retain and occupy the new one; yield after a failed position test.
// Plan phase 4 (2026-11-05), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Retail0x00970CD0,277 bytes. Uses shared UseCoverBase, Base and Brain storage. Inherited enemy handle+0x14 is opaque in the generated group header.
// CThing retail component map+0x44 differs from donor+0x48; narrow retail shell preserves audited fields. Brain dispatch slot2 is retained through a methods-only view of the shared header's vtable prefix.
#include "engine/CAIStateGroup_CreatureUseCoverBase.h"
#include "engine/CAIStateGroupBase.h"
#include "engine/CAIBrain.h"
#include "engine/C3DVector.h"
struct CThingRetail;
class CBaseObject;
class CBaseIntelligentPointer {
public:
    void SetPItem(CBaseObject*a0);   // 0x00A01B90 (Ego_r.pdb ?SetPItem@CBaseIntelligentPointer@@QAEXPAVCBaseObject@@@Z)
private: public:
    CBaseObject* GetPItem() const;   // 0x00A01B50 (Ego_r.pdb ?GetPItem@CBaseIntelligentPointer@@QBEPAVCBaseObject@@XZ)
};
struct CIntelligentPointerRetail { virtual void v00(); void* PData;   }; //00A01B50/00A01B90
struct CIntelligentVector { CIntelligentPointerRetail* first; CIntelligentPointerRetail* last; CIntelligentPointerRetail* capacity; unsigned char allocators[4];
 struct iterator { CIntelligentPointerRetail* p; explicit iterator(CIntelligentPointerRetail* q):p(q){} CIntelligentPointerRetail& operator*()const{return *p;} iterator operator++(int){iterator old(p);++p;return old;} bool operator!=(const iterator& other)const{return p!=other.p;} };
 iterator begin(){return iterator(first);} iterator end(){return iterator(last);} bool empty()const{return first==last;}
};
struct CBrainDispatch { virtual void v00();virtual void v01();virtual bool SuspendSuspendableProcess(); }; //slot2, 0x00A44840 verified in CAIBrain vtable0x012780C4
struct CTCOccupiableMethods { bool IsOccupied(); void SetOccupier(CThingRetail*); }; //0087C3F0,0087C390
class CTCBase;
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:typedef std::pair<ETCInterfaceType, CTCBase*> Entry;Entry* first;Entry* last;Entry* LowerBound(const ETCInterfaceType& key);};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap; //0040F020
struct CThingRetail {
 unsigned char inherited[0x20];unsigned long UsingComponents[9];CInterfaceMap Components;
 unsigned char beforeDeath[0x91-0x4c];unsigned char deathFlags;
 __forceinline bool GetOccupiable(CTCOccupiableMethods** out) {
  if(UsingComponents[5]&0x1000) {
   ETCInterfaceType key = (ETCInterfaceType)(172);CInterfaceMap* map=&Components;CInterfaceMap::Entry* it=map->LowerBound(key);
   if(it==map->last || 172<it->first)it=map->last;
   *out=(CTCOccupiableMethods*)((void*)it->second);return true;
  }return false;
 }
};
struct UseCoverMethods : CAIStateGroup_CreatureUseCoverBase {
 __forceinline CIntelligentPointerRetail& Enemy(){return *(CIntelligentPointerRetail*)_pad_0x14;}
 __forceinline CIntelligentPointerRetail& Cover(){return *(CIntelligentPointerRetail*)PCoverThing;}
 __forceinline CIntelligentVector& Candidates(){return *(CIntelligentVector*)CoverList;}
 bool GetHiddenCoverPos(C3DVector&,CThingRetail*); //00970930
 void ClearCover(); //00970B60
 bool GetHiddenCoverPosAndSetUpCoverThing(C3DVector&);
};
bool UseCoverMethods::GetHiddenCoverPosAndSetUpCoverThing(C3DVector& result)
{
 if(Candidates().empty())return false;
 CIntelligentVector::iterator it=Candidates().begin();
 do {
  if(!((CThingRetail*)((CBaseIntelligentPointer*)&(Enemy()))->GetPItem()) || (((CThingRetail*)((CBaseIntelligentPointer*)&(Enemy()))->GetPItem())->deathFlags&1))return false;
  CIntelligentPointerRetail& candidate=*it++;
  if(((CThingRetail*)((CBaseIntelligentPointer*)&candidate)->GetPItem()) && !(((CThingRetail*)((CBaseIntelligentPointer*)&candidate)->GetPItem())->deathFlags&1)) {
   if(GetHiddenCoverPos(result,((CThingRetail*)((CBaseIntelligentPointer*)&candidate)->GetPItem()))) {
    CTCOccupiableMethods* occupiable;
    if(((CThingRetail*)((CBaseIntelligentPointer*)&candidate)->GetPItem())->GetOccupiable(&occupiable) && !occupiable->IsOccupied()) {
     ClearCover();
     ((CBaseIntelligentPointer*)&(Cover()))->SetPItem((CBaseObject*)(((CThingRetail*)((CBaseIntelligentPointer*)&candidate)->GetPItem())));
     occupiable->SetOccupier((CThingRetail*)((CAIStateGroupBase*)this)->PBrain->Creature);
     return true;
    }
   }else ((CBrainDispatch*)((CAIStateGroupBase*)this)->PBrain)->SuspendSuspendableProcess();
  }
 }while(it!=Candidates().end());
 return false;
}