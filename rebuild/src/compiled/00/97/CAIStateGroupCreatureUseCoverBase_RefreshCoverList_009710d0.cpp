// CAIStateGroup_CreatureUseCoverBase::RefreshCoverList() -- refill retained cover handles from a filtered temporary list.
// Retail009710D0,250 bytes. Retail vector is12 bytes versus donor16: ValidPositionsFound lives at +0x48 (engine header, retail STLport 4.6 layout).
#include <stddef.h>
#include <stdlib.h>
#include "engine/CAIStateGroup_CreatureUseCoverBase.h"
inline void* operator new(size_t,void* p){return p;}
struct CThingRetail;
struct CIntelligentPointerBaseRetail {virtual ~CIntelligentPointerBaseRetail();void* Data; CIntelligentPointerBaseRetail(); CThingRetail* GetPItem()const;void SetPItem(CThingRetail*);}; //A01B10,A01B50,A01B90,A01C10
struct CIntelligentPointerRetail:CIntelligentPointerBaseRetail {
 __forceinline CIntelligentPointerRetail(CThingRetail* p){SetPItem(p);}
 __forceinline CIntelligentPointerRetail(const CIntelligentPointerRetail& p){SetPItem(p.GetPItem());}
};
struct CAllocatorTag {CAllocatorTag(){}};
struct CHandleVector {CIntelligentPointerRetail* first;CIntelligentPointerRetail* last;CIntelligentPointerRetail* capacity;
 void Erase(CIntelligentPointerRetail*,CIntelligentPointerRetail*); //004AC0F0
 void InsertOverflow(CIntelligentPointerRetail*,const CIntelligentPointerRetail&,const CAllocatorTag&,unsigned long,bool); //004AC940
 __forceinline void clear(){Erase(first,last);}
 __forceinline void push_back(const CIntelligentPointerRetail& p){if(last!=capacity){if(last)new(last)CIntelligentPointerRetail(p);++last;}else InsertOverflow(last,p,CAllocatorTag(),1,true);}
};
struct CThingVector {CThingRetail** first;CThingRetail** last;CThingRetail** capacity;
 CThingVector():first(0),last(0),capacity(0){}
 ~CThingVector(){if(first)free(first);}
 bool empty()const{return first==last;}
};
struct CCoverDispatch {
 virtual void s00();virtual void s01();virtual void s02();virtual void s03();virtual void s04();virtual void s05();virtual void s06();virtual void s07();virtual void s08();virtual void s09();virtual void s10();virtual void s11();virtual void s12();virtual void s13();virtual void s14();virtual void s15();virtual void s16();virtual void s17();virtual void s18();virtual void s19();virtual void s20();virtual void s21();
 virtual void FilterCoverList(CThingVector&);virtual void GetCoverThingList(CThingVector&);
};
struct UseCoverMethods:CAIStateGroup_CreatureUseCoverBase {void RefreshCoverList(); __forceinline CHandleVector& Covers(){return *(CHandleVector*)CoverList;} __forceinline long& RetailValidPositionsFound(){return ValidPositionsFound;}};
void UseCoverMethods::RefreshCoverList(){
 Covers().clear(); RetailValidPositionsFound()=0;
 CThingVector things;
 ((CCoverDispatch*)this)->GetCoverThingList(things);
 if(!things.empty()){
  ((CCoverDispatch*)this)->FilterCoverList(things);
  for(CThingRetail** it=things.first;it!=things.last;++it)Covers().push_back(CIntelligentPointerRetail(*it));
 }
}