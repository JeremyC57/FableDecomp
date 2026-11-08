// CAIStateGroup_CreatureUseCoverBase::RefreshCoverList() -- refill retained cover handles from a filtered temporary list.
// Plan phase 5 (2026-11-05), CIntelligentPointer wave: the local CBaseIntelligentPointer / CIntelligentPointer stand-ins replaced by the shared engine/CIntelligentPointer.h, retail (byte-packed) branch FABLE_RETAIL_CINTELLIGENTPOINTER (Ego_r.pdb: vptr +0x0, PData +0x4; ctor 0x00A01B10, dtor 0x00A01C10, GetPItem 0x00A01B50, SetPItem 0x00A01B90).
// Retail009710D0,250 bytes. Retail vector is12 bytes versus donor16: ValidPositionsFound lives at +0x48 (engine header, retail STLport 4.6 layout).
#include "fable_compiler.h"
#include <stddef.h>
#include <stdlib.h>
#include "engine/CAIStateGroup_CreatureUseCoverBase.h"
inline void* operator new(size_t,void* p){return p;}
class CBaseObject;
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"
struct CIntelligentPointerRetail:CBaseIntelligentPointer {
 FABLE_FORCEINLINE CIntelligentPointerRetail(CBaseObject* p){SetPItem(p);}
 FABLE_FORCEINLINE CIntelligentPointerRetail(const CIntelligentPointerRetail& p){SetPItem(p.GetPItem());}
};
struct CAllocatorTag {CAllocatorTag(){}};
struct CHandleVector {CIntelligentPointerRetail* first;CIntelligentPointerRetail* last;CIntelligentPointerRetail* capacity;
 void Erase(CIntelligentPointerRetail*,CIntelligentPointerRetail*); //004AC0F0
 void InsertOverflow(CIntelligentPointerRetail*,const CIntelligentPointerRetail&,const CAllocatorTag&,unsigned long,bool); //004AC940
 FABLE_FORCEINLINE void clear(){Erase(first,last);}
 FABLE_FORCEINLINE void push_back(const CIntelligentPointerRetail& p){if(last!=capacity){if(last)new(last)CIntelligentPointerRetail(p);++last;}else InsertOverflow(last,p,CAllocatorTag(),1,true);}
};
struct CThingVector {CBaseObject** first;CBaseObject** last;CBaseObject** capacity;
 CThingVector():first(0),last(0),capacity(0){}
 ~CThingVector(){if(first)free(first);}
 bool empty()const{return first==last;}
};
struct CCoverDispatch {
 virtual void s00();virtual void s01();virtual void s02();virtual void s03();virtual void s04();virtual void s05();virtual void s06();virtual void s07();virtual void s08();virtual void s09();virtual void s10();virtual void s11();virtual void s12();virtual void s13();virtual void s14();virtual void s15();virtual void s16();virtual void s17();virtual void s18();virtual void s19();virtual void s20();virtual void s21();
 virtual void FilterCoverList(CThingVector&);virtual void GetCoverThingList(CThingVector&);
};
struct UseCoverMethods:CAIStateGroup_CreatureUseCoverBase {void RefreshCoverList(); FABLE_FORCEINLINE CHandleVector& Covers(){return *(CHandleVector*)CoverList;} FABLE_FORCEINLINE long& RetailValidPositionsFound(){return ValidPositionsFound;}};
void UseCoverMethods::RefreshCoverList(){
 Covers().clear(); RetailValidPositionsFound()=0;
 CThingVector things;
 ((CCoverDispatch*)this)->GetCoverThingList(things);
 if(!things.empty()){
  ((CCoverDispatch*)this)->FilterCoverList(things);
  for(CBaseObject** it=things.first;it!=things.last;++it)Covers().push_back(CIntelligentPointerRetail(*it));
 }
}