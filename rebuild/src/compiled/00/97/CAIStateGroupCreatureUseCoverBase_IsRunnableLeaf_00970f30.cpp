// CAIStateGroup_CreatureUseCoverBase::IsRunnableLeaf() -- after a failed cover search wait 8 seconds or 15 units of movement; then gather the cover things: none records the failure here, otherwise filter them into the retained cover handles
// Leaf: IsRunnableLeaf@CAIStateGroup_CreatureUseCoverBase
// Model: the landed sibling RefreshCoverList 0x009710D0 (same TU). Layout: engine/CAIStateGroup_CreatureUseCoverBase.h (retail
//   STLport 4.6: 12-byte CoverList): FailedToFindCover +0x34, FailedTime +0x38, FailedToFindCoverAt +0x3c; PBrain +4.
// Shape: C3DVector offset (operator-, Z dead) keeps retail frame slots; positive !empty() block puts the failure path last.
#include <stddef.h>
#include <stdlib.h>
#include "engine/CAIStateGroup_CreatureUseCoverBase.h"
inline void* operator new(size_t,void* p){return p;}
class C3DVector { public:
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    C3DVector operator-(const C3DVector& o) const { return C3DVector(X - o.X, Y - o.Y, Z - o.Z); }
    float GetLengthSquared2D() const { return X * X + Y * Y; }
};
class CThing { public: const C3DVector& GetPos() const; };
struct CThingRetail : public CThing { public:  };   // 0x004C73D0
struct CAIBrainRetail { unsigned char _pad[0x18]; CThingRetail* Creature; };   // +0x18
class CWorld { public: static long GetFramesPerSecond(); };
extern long g_framesPerSecond;                  // 0x01375550
inline long CWorld::GetFramesPerSecond() { return g_framesPerSecond; }
long __fastcall GFGetCurrentFrame();            // 0x0049D870
struct CIntelligentPointerBaseRetail {virtual ~CIntelligentPointerBaseRetail();void* Data; CIntelligentPointerBaseRetail(); CThingRetail* GetPItem()const;void SetPItem(CThingRetail*);}; //A01B10,A01B50,A01B90,A01C10
struct CIntelligentPointerRetail:CIntelligentPointerBaseRetail {
 __forceinline CIntelligentPointerRetail(CThingRetail* p){SetPItem(p);}
 __forceinline CIntelligentPointerRetail(const CIntelligentPointerRetail& p){SetPItem(p.GetPItem());}
};
struct CAllocatorTag {CAllocatorTag(){}};
struct CHandleVector {CIntelligentPointerRetail* first;CIntelligentPointerRetail* last;CIntelligentPointerRetail* capacity;
 void InsertOverflow(CIntelligentPointerRetail*,const CIntelligentPointerRetail&,const CAllocatorTag&,unsigned long,bool); //004AC940
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
struct UseCoverMethods:CAIStateGroup_CreatureUseCoverBase {
 bool IsRunnableLeaf();
 CWorld* GetPWorld();                           // 0x008FCFC0
 __forceinline CThingRetail* GetCreature(){return (*(CAIBrainRetail**)_pad_0x04)->Creature;}
 __forceinline CHandleVector& Covers(){return *(CHandleVector*)CoverList;}
 __forceinline bool& RetailFailedToFindCover(){return FailedToFindCover;}
 __forceinline long& RetailFailedTime(){return FailedTime;}
 __forceinline C3DVector& RetailFailedToFindCoverAt(){return *(C3DVector*)&FailedToFindCoverAt_X;}
};
bool UseCoverMethods::IsRunnableLeaf(){
 if(RetailFailedToFindCover()){
  C3DVector offset=RetailFailedToFindCoverAt()-GetCreature()->GetPos();
  long framesPerSecond=GetPWorld()->GetFramesPerSecond();
  long seconds=(GFGetCurrentFrame()-RetailFailedTime())/framesPerSecond;
  if(offset.GetLengthSquared2D()<225.0f&&seconds<8)return false;
  RetailFailedToFindCover()=false;
 }
 CThingVector things;
 ((CCoverDispatch*)this)->GetCoverThingList(things);
 if(!things.empty()){
  ((CCoverDispatch*)this)->FilterCoverList(things);
  for(CThingRetail** it=things.first;it!=things.last;++it)Covers().push_back(CIntelligentPointerRetail(*it));
  return true;
 }
 RetailFailedToFindCoverAt()=GetCreature()->GetPos();
 RetailFailedToFindCover()=true;
 return false;
}