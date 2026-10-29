#ifndef _MSC_VER
#define __int64 int __attribute__((mode(DI)))
#define __fastcall
#endif
#include <stddef.h>
extern "C" int puts(const char*);
// 0090df60 CMessageEventFilter_ThingSearchFilter<CThingFilter_In2DRange>::operator()(const CMessageEvent&) const
// Leaf: ??R?$CMessageEventFilter_ThingSearchFilter@VCThingFilter_In2DRange@@@@QBE_NABVCMessageEvent@@@Z
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
class CBaseClass {public:virtual~CBaseClass();};
class CBaseObject:public CBaseClass {
protected:virtual~CBaseObject();
private:unsigned char unobservedObjectState[4];
};
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
class CBaseIntelligentPointer {
public:virtual~CBaseIntelligentPointer();CBaseObject*GetPItem()const;
private:unsigned char unobservedPointerState[4];
};
template<class T>class CIntelligentPointer:public CBaseIntelligentPointer {};
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
class CThingFilter_In2DRange {public:bool operator()(const CThing&)const;};
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
template bool CMessageEventFilter_ThingSearchFilter<CThingFilter_In2DRange>::operator()(const CMessageEvent&)const;

static CBaseObject*resolved[3];
static int resolutionCount;
static int filterCount;
static bool filterAnswer;
static const CThing*filterThing;
static const CBaseIntelligentPointer*expectedPointer;
static bool badPointer;
CBaseClass::~CBaseClass(){}
CBaseObject::~CBaseObject(){}
CBaseIntelligentPointer::~CBaseIntelligentPointer(){}
CBaseObject*CBaseIntelligentPointer::GetPItem()const {
    if(this!=expectedPointer)badPointer=true;
    if(resolutionCount>=3)return 0;
    return resolved[resolutionCount++];
}
bool CThingFilter_In2DRange::operator()(const CThing&thing)const {
    ++filterCount;filterThing=&thing;return filterAnswer;
}
static int runCase(unsigned long flags,bool first,bool third,bool answer) {
    CThingAICreature thing;
    reinterpret_cast<CThingLayout*>(&thing)->ThingTypeFlags=flags;
    CMessageEvent event;CThingFilter_In2DRange inner;
    CMessageEventFilter_ThingSearchFilter<CThingFilter_In2DRange>candidate(inner);
    resolved[0]=first?static_cast<CBaseObject*>(&thing):0;
    resolved[1]=static_cast<CBaseObject*>(&thing);
    resolved[2]=third?static_cast<CBaseObject*>(&thing):0;
    expectedPointer=reinterpret_cast<const CBaseIntelligentPointer*>(reinterpret_cast<const unsigned char*>(&event)+72);
    resolutionCount=0;filterCount=0;filterAnswer=answer;filterThing=0;badPointer=false;
    bool actual=candidate(event);
    bool allSet=true;
    for(unsigned bit=0;bit!=5;++bit)if((flags&(1UL<<bit))==0)allSet=false;
    bool expected=first&&allSet&&third&&answer;
    int expectedResolutions=!first?1:(!allSet?2:3);
    int expectedCalls=first&&allSet&&third?1:0;
    return actual==expected&&resolutionCount==expectedResolutions&&filterCount==expectedCalls&&!badPointer&&(!expectedCalls||filterThing==static_cast<CThing*>(&thing))?0:1;
}
int main(){
    if(sizeof(CIntelligentPointer<CThing>)!=8||sizeof(CMessageEvent)!=88||sizeof(CThingAICreature)!=sizeof(CThingAICreatureLayout))return 1;
    for(unsigned flags=0;flags!=128;++flags)for(unsigned bits=0;bits!=8;++bits)
        if(runCase(flags,(bits&1)!=0,(bits&2)!=0,(bits&4)!=0))return 2;
    if(runCase(0xffffffffUL,true,true,true))return 3;
    puts("0090df60_TEST PASS");return 0;
}