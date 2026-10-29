// CAIStateGroup_MinionReactToTrespassing::IsStillRunnable(bool) -- periodically check ongoing trespass deeds targeting the creature home
// Retail0x009580e0,295 bytes. Home retail+168 differs from donor+190, hence local creature overlay.
#include "engine/CAIStateGroup_MinionReactToTrespassing.h"
#include "engine/CAIBrain.h"
#include "engine/CTCCreatureOpinionOfHero.h"
class CThing;
struct CBaseIntelligentPointer { virtual void PointerType(); void* Handle; CThing* GetPItem() const; }; // 0xA01B50
class CTCBase;
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public: typedef std::pair<ETCInterfaceType, CTCBase*> Entry; Entry *first,*last; const Entry* LowerBound(const ETCInterfaceType& key) const; };
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
class CThing { public:
    unsigned char prefix[0x20]; unsigned long bits[9]; CInterfaceMap interfaces;
    unsigned char middle[0x45]; unsigned char dead; unsigned char tail[0xd6]; CBaseIntelligentPointer Home; CThing* GetHome() { return Home.GetPItem(); }
    template<class T> __forceinline bool GetInterface(T** out,long type) {
        if(bits[type>>5]&(1UL<<(type&31))) {
            ETCInterfaceType key = (ETCInterfaceType)(type); CInterfaceMap* map=&interfaces; const CInterfaceMap::Entry* it=map->LowerBound(key);
            if(it==map->last || it->first>type) it=map->last;
            *out=(T*)((void*)it->second); return true;
        } return false;
    }
};


class CWorld { public: static long __fastcall GetFrame(); }; // 0x49D870
extern long g_TrespassCheckInterval; // 0x138D2D4
class COpinionDeedFilter_IsStillHappening { public: COpinionDeedFilter_IsStillHappening() {} };
class COpinionDeedFilter_TargetIs { public: CThing* Target; COpinionDeedFilter_TargetIs(CThing* p):Target(p){} };
struct CFilter_Bind2 {
 const COpinionDeedFilter_IsStillHappening* F1; const COpinionDeedFilter_TargetIs* F2;
 CFilter_Bind2(const COpinionDeedFilter_IsStillHappening& a,const COpinionDeedFilter_TargetIs& b):F1(&a),F2(&b){}
};
struct CTCCreatureOpinionOfHeroMethods : CTCCreatureOpinionOfHero { void* FindMostRecentDeed(long,const CFilter_Bind2&,long*,bool); }; // 0x92E630
struct CAIStateGroup_MinionReactToTrespassing_Adapter : CAIStateGroup_MinionReactToTrespassing {
 CAIBrain* Brain() { return *(CAIBrain**)&_pad_0x04[0]; }
 bool IsStillRunnable(bool initialCheck);
};
bool CAIStateGroup_MinionReactToTrespassing_Adapter::IsStillRunnable(bool initialCheck) {
 if(!initialCheck && CWorld::GetFrame()-LastFrameCheckedIsStillValid < g_TrespassCheckInterval) return true;
 LastFrameCheckedIsStillValid=CWorld::GetFrame();
 CThing* home=((CThing*)Brain()->Creature)->GetHome();
 if(home && !(home->dead&1)) {
  CTCCreatureOpinionOfHeroMethods* opinion;
  if(((CThing*)Brain()->Creature)->GetInterface(&opinion,0x4f)) {
   if(opinion->FindMostRecentDeed(10,CFilter_Bind2(COpinionDeedFilter_IsStillHappening(),COpinionDeedFilter_TargetIs(home)),0,true) ||
      opinion->FindMostRecentDeed(11,CFilter_Bind2(COpinionDeedFilter_IsStillHappening(),COpinionDeedFilter_TargetIs(home)),0,true) ||
      opinion->FindMostRecentDeed(2,CFilter_Bind2(COpinionDeedFilter_IsStillHappening(),COpinionDeedFilter_TargetIs(home)),0,true)) return true;
  }
 }
 return false;
}