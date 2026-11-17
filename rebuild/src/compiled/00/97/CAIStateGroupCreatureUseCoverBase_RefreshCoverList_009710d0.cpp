// CAIStateGroup_CreatureUseCoverBase::RefreshCoverList() -- refill retained cover handles from a filtered temporary list.
// Plan phase 5 (2026-11-05), CIntelligentPointer wave: the local CBaseIntelligentPointer / CIntelligentPointer stand-ins replaced by the shared engine/CIntelligentPointer.h, retail (byte-packed) branch FABLE_RETAIL_CINTELLIGENTPOINTER (Ego_r.pdb: vptr +0x0, PData +0x4; ctor 0x00A01B10, dtor 0x00A01C10, GetPItem 0x00A01B50, SetPItem 0x00A01B90).
// Retail009710D0,250 bytes. Retail vector is12 bytes versus donor16: ValidPositionsFound lives at +0x48 (engine header, retail STLport 4.6 layout).
#include "fable_compiler.h"
#include <stddef.h>
#include <stdlib.h>
#include "engine/CAIStateGroup_CreatureUseCoverBase.h"
// Name fix (agentNF54): the local CIntelligentPointerRetail / CAllocatorTag / CHandleVector stand-ins replaced by the shared
//   engine/CIntelligentPointer.h (retail branch, FABLE_RETAIL_CINTELLIGENTPOINTER_COPY) over engine/CThing.h and the STLport 4.6 vector of
//   stlport/fable_containers.h (erase(first, last) out of line, FABLE_RETAIL_STLP_VECTOR_ERASE_RANGE_OUTOFLINE): CoverList is the PDB
//   vector<CIntelligentPointer<CThing> > (engine header comment), so the overflow call 0x004AC940 binds
//   ?_M_insert_overflow@?$vector@V?$CIntelligentPointer@VCThing@@@@V?$allocator@V?$CIntelligentPointer@VCThing@@@@@std@@@std@@IAEXPAV?$CIntelligentPointer@VCThing@@@@ABV3@ABU__false_type@2@I_N@Z
//   (the landed row's own leaf; was ?InsertOverflow@CHandleVector@@...) and clear()'s erase 0x004AC0F0 the STLport erase (was CHandleVector::Erase).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COPY
#define FABLE_RETAIL_STLP_VECTOR_ERASE_RANGE_OUTOFLINE
#include "stlport/fable_containers.h"
#include "engine/CThing.h"
#include "engine/CIntelligentPointer.h"
struct CThingVector {CBaseObject** first;CBaseObject** last;CBaseObject** capacity;
 CThingVector():first(0),last(0),capacity(0){}
 ~CThingVector(){if(first)free(first);}
 bool empty()const{return first==last;}
};
struct CCoverDispatch {
 virtual void s00();virtual void s01();virtual void s02();virtual void s03();virtual void s04();virtual void s05();virtual void s06();virtual void s07();virtual void s08();virtual void s09();virtual void s10();virtual void s11();virtual void s12();virtual void s13();virtual void s14();virtual void s15();virtual void s16();virtual void s17();virtual void s18();virtual void s19();virtual void s20();virtual void s21();
 virtual void FilterCoverList(CThingVector&);virtual void GetCoverThingList(CThingVector&);
};
struct UseCoverMethods:CAIStateGroup_CreatureUseCoverBase {void RefreshCoverList(); FABLE_FORCEINLINE std::vector<CIntelligentPointer<CThing> >& Covers(){return *(std::vector<CIntelligentPointer<CThing> >*)CoverList;} FABLE_FORCEINLINE long& RetailValidPositionsFound(){return ValidPositionsFound;}};
void UseCoverMethods::RefreshCoverList(){
 Covers().clear(); RetailValidPositionsFound()=0;
 CThingVector things;
 ((CCoverDispatch*)this)->GetCoverThingList(things);
 if(!things.empty()){
  ((CCoverDispatch*)this)->FilterCoverList(things);
  for(CBaseObject** it=things.first;it!=things.last;++it)Covers().push_back(CIntelligentPointer<CThing>((CThing*)*it));
 }
}