// 0090dfb0 CMessageEventFilter_ThingSearchFilter<CThingFilter_IsNavLineOfSightToThing>::operator()(const CMessageEvent&) const
// Leaf: ??R?$CMessageEventFilter_ThingSearchFilter@VCThingFilter_IsNavLineOfSightToThing@@@@QBE_NABVCMessageEvent@@@Z
// Plan phase 5 (2026-11-05), CIntelligentPointer wave: the local CBaseIntelligentPointer / CIntelligentPointer stand-ins replaced by the shared engine/CIntelligentPointer.h, retail (byte-packed) branch FABLE_RETAIL_CINTELLIGENTPOINTER (Ego_r.pdb: vptr +0x0, PData +0x4; ctor 0x00A01B10, dtor 0x00A01C10, GetPItem 0x00A01B50, SetPItem 0x00A01B90).
// Flags: /O2 /Ob2 /Oy /GS- /GR- /EHsc
// Layout: original inheritance with opaque unread state; Thing flags accessed only through documented physical alias.
#define CClass CClassLayout
#include "engine/CClass.h"
#undef CClass
#define CThing CThingLayout
#define CTCGraphicAppearance CTCGraphicAppearanceLayout
#include "engine/_quarantine/CThing.h"
#undef CThing
#undef CTCGraphicAppearance
#define CThingPhysical CThingPhysicalLayout
#include "engine/CThingPhysical.h"
#undef CThingPhysical
#define CThingGameObject CThingGameObjectLayout
#include "engine/CThingGameObject.h"
#undef CThingGameObject
#define CThingCreatureBase CThingCreatureBaseLayout
#include "engine/CThingCreatureBase.h"
#undef CThingCreatureBase
#define CThingAICreature CThingAICreatureLayout
#include "engine/CThingAICreature.h"
#undef CThingAICreature

namespace std {
template<class A,class R>struct unary_function {typedef A argument_type;typedef R result_type;};
}
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0 (slot 0), implicit copy
#define FABLE_RETAIL_BASE_OBJECT
#include "engine/CBaseObject.h"                       // : CBaseClass, vtable 0x0129A7D8 (~ slot 0, Validate slot 1)
#pragma pack(push,1)
class CClass:public CBaseObject {private:unsigned char unobservedClassState[sizeof(CClassLayout)-sizeof(CBaseObject)];};
class CThing:public CClass {
public:
    unsigned long GetTypeFlags()const{return reinterpret_cast<const CThingLayout*>(this)->ThingTypeFlags;}
private:unsigned char unobservedThingState[sizeof(CThingLayout)-sizeof(CClass)];
};
class CThingPhysical:public CThing {private:unsigned char unobservedPhysicalState[sizeof(CThingPhysicalLayout)-sizeof(CThing)];};
class CThingGameObject:public CThingPhysical {private:unsigned char unobservedGameObjectState[sizeof(CThingGameObjectLayout)-sizeof(CThingPhysical)];};
class CThingCreatureBase:public CThingGameObject {private:unsigned char unobservedCreatureState[sizeof(CThingCreatureBaseLayout)-sizeof(CThingGameObject)];};
class CThingAICreature:public CThingCreatureBase {
public:static unsigned long __fastcall StatGetTypeFlags(){return 31;}
private:unsigned char unobservedAIState[sizeof(CThingAICreatureLayout)-sizeof(CThingCreatureBase)];
};
#pragma pack(pop)
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"
class CMessageEvent:public CBaseObject {
public:
    template<class T>T*GetPOriginator()const {
        if(!POriginator.GetPItem())return 0;
        if((static_cast<CThing*>(POriginator.GetPItem())->GetTypeFlags()^T::StatGetTypeFlags())&T::StatGetTypeFlags())return 0;
        return static_cast<T*>(POriginator.GetPItem());
    }
private:
    unsigned char unobservedEventState[72-sizeof(CBaseObject)];
    CIntelligentPointer<CThing>POriginator;
    unsigned char unobservedTargetState[8];
};
class CThingFilter_IsNavLineOfSightToThing {public:bool operator()(const CThing&)const;};
template<class F>class CMessageEventFilter_ThingSearchFilter:public std::unary_function<CMessageEvent,bool> {
public:
    CMessageEventFilter_ThingSearchFilter(const F&filter):Filter(&filter){}
    bool operator()(const CMessageEvent&)const;
private:const F*Filter;
};
template<class F>bool CMessageEventFilter_ThingSearchFilter<F>::operator()(const CMessageEvent&message)const
{
    CThingAICreature*thing=message.GetPOriginator<CThingAICreature>();
    if(!thing)return false;
    return (*Filter)(*thing);
}
template bool CMessageEventFilter_ThingSearchFilter<CThingFilter_IsNavLineOfSightToThing>::operator()(const CMessageEvent&)const;