// Deterministic dependencies for the exact selected initializer body.
#include <cassert>
#include <cstdio>
#include <cstring>
#include <vector>
struct C2DCoordI{long X,Y;C2DCoordI(long x,long y):X(x),Y(y){}};
struct C3DVector{float X,Y,Z;C3DVector(float x=0,float y=0,float z=0):X(x),Y(y),Z(z){}C2DCoordI ToC2DCoordI()const{return C2DCoordI((long)X,(long)Y);}};
struct C2DBoxI{long TLX,TLY,BRX,BRY;C2DBoxI(const C2DCoordI& p,long r):TLX(p.X-r),TLY(p.Y-r),BRX(p.X+r),BRY(p.Y+r){}};
class CCharString{public:const char* text;CCharString(const char*s):text(s){}};
enum EScriptAIPriority{SCRIPT_LOW=1,SCRIPT_HIGH=4};
class CTCTrapBase{public:int deactivations;CTCTrapBase():deactivations(0){}void ManualDeactivate(){++deactivations;}};
class CThing{public:bool dead;int refs;C3DVector pos;CTCTrapBase* trap;CThing(bool d=false,CTCTrapBase*t=0):dead(d),refs(0),pos(-1.75f,5.5f,2.0f),trap(t){}const C3DVector&GetPos()const{return pos;}template<class T>bool GetTC(T** out){if(!trap)return false;*out=static_cast<T*>(trap);return true;}};
class CThingAICreature:public CThing{};
class CAIBrain{};
template<class T>bool IS_THING_ALIVE(T* thing){return thing&&!thing->dead;}
template<class T>class CIntelligentPointer{T* p;public:CIntelligentPointer(T*t):p(t){if(p)++p->refs;}CIntelligentPointer(const CIntelligentPointer&o):p(o.p){if(p)++p->refs;}~CIntelligentPointer(){if(p)--p->refs;}T*Get()const{return p;}};
class CThingFilter_HasDefGlobalIndex{public:long index;explicit CThingFilter_HasDefGlobalIndex(long n):index(n){}};
class CGameDefinitionManager{public:int calls;CGameDefinitionManager():calls(0){}static CGameDefinitionManager&Get(){static CGameDefinitionManager x;return x;}long GetDefGlobalIndexFromName(const CCharString&s){++calls;if(std::strcmp(s.text,"OBJECT_HERO_ARENA_SPEAR_TRAP_01")==0)return 101;assert(std::strcmp(s.text,"OBJECT_HERO_ARENA_BLADES_TRAP_01")==0);return 202;}};
class CThingSearchTools{public:std::vector<CThing*>spear,blade;mutable int calls;CThingSearchTools():calls(0){}template<class T,class Container,class Filter>void GetAllThingsInRoughArea(const C2DBoxI&area,const Filter&filter,Container&out)const{++calls;assert(area.TLX==-41&&area.TLY==-35&&area.BRX==39&&area.BRY==45);const std::vector<CThing*>&src=filter.index==101?spear:blade;for(unsigned int n=0;n<src.size();++n)out.push_back(src[n]);}};
class CAIStateGroupBase{protected:CThingAICreature*owner;public:int initialized;long lastPriority;EScriptAIPriority lastScriptPriority;CAIStateGroupBase():owner(0),initialized(0),lastPriority(0),lastScriptPriority(SCRIPT_LOW){}void Initialise(CAIBrain&,CThingAICreature&creature,long p,EScriptAIPriority sp,const CCharString&){owner=&creature;++initialized;lastPriority=p;lastScriptPriority=sp;}CThingAICreature&GetCreature()const{return *owner;}};
class CAIStateGroup_ScorpionKing:public CAIStateGroupBase{public:CThingSearchTools search;std::vector<CIntelligentPointer<CThing> >TrapThings;long ChargeCount;CAIStateGroup_ScorpionKing():ChargeCount(99){}const CThingSearchTools&PeekThingSearchTools()const{return search;}void Initialise(CAIBrain&,CThingAICreature&,long,EScriptAIPriority,const CCharString&);};

void CAIStateGroup_ScorpionKing::Initialise(CAIBrain& brain,CThingAICreature& creature,long priority,EScriptAIPriority script_ai_priority,const CCharString& name)
{
    CAIStateGroupBase::Initialise(brain,creature,priority,script_ai_priority,name);
    std::vector<CThing*> things;
    long speartrap_def_index=CGameDefinitionManager::Get().GetDefGlobalIndexFromName("OBJECT_HERO_ARENA_SPEAR_TRAP_01");
    long bladetrap_def_index=CGameDefinitionManager::Get().GetDefGlobalIndexFromName("OBJECT_HERO_ARENA_BLADES_TRAP_01");
    PeekThingSearchTools().GetAllThingsInRoughArea<CThing>(C2DBoxI(GetCreature().GetPos().ToC2DCoordI(),40),CThingFilter_HasDefGlobalIndex(speartrap_def_index),things);
    PeekThingSearchTools().GetAllThingsInRoughArea<CThing>(C2DBoxI(GetCreature().GetPos().ToC2DCoordI(),40),CThingFilter_HasDefGlobalIndex(bladetrap_def_index),things);
    long n;
    for(n=0;n<things.size();++n) {
        if(IS_THING_ALIVE(things[n])) {
            TrapThings.push_back(things[n]);
            CTCTrapBase* ptc_trap;
            if(things[n]->GetTC(&ptc_trap))ptc_trap->ManualDeactivate();
        }
    }
    ChargeCount=0;
}
int main(){
    CAIBrain brain;CThingAICreature creature;
    {CAIStateGroup_ScorpionKing s;s.Initialise(brain,creature,7,SCRIPT_HIGH,"test");assert(s.initialized==1&&s.lastPriority==7&&s.lastScriptPriority==SCRIPT_HIGH);assert(s.search.calls==2&&s.TrapThings.empty()&&s.ChargeCount==0);}
    CTCTrapBase tc1,tc2;CThing live1(false,&tc1),dead(true,&tc2),noTC(false,0),live2(false,&tc2);
    {CAIStateGroup_ScorpionKing s;s.search.spear.push_back(0);s.search.spear.push_back(&dead);s.search.spear.push_back(&live1);s.search.blade.push_back(&noTC);s.search.blade.push_back(&live2);s.search.blade.push_back(&live1);s.Initialise(brain,creature,-3,SCRIPT_LOW,"another");assert(s.ChargeCount==0&&s.TrapThings.size()==4);assert(s.TrapThings[0].Get()==&live1&&s.TrapThings[1].Get()==&noTC&&s.TrapThings[2].Get()==&live2&&s.TrapThings[3].Get()==&live1);assert(tc1.deactivations==2&&tc2.deactivations==1);assert(live1.refs==2&&noTC.refs==1&&live2.refs==1&&dead.refs==0);}
    assert(live1.refs==0&&noTC.refs==0&&live2.refs==0&&dead.refs==0);
    {CAIStateGroup_ScorpionKing s;s.TrapThings.push_back(&live2);s.search.spear.push_back(&live1);s.Initialise(brain,creature,0,SCRIPT_LOW,"retain");assert(s.TrapThings.size()==2&&s.TrapThings[0].Get()==&live2&&s.TrapThings[1].Get()==&live1);assert(tc1.deactivations==3);}
    assert(live1.refs==0&&live2.refs==0);assert(CGameDefinitionManager::Get().calls==6);std::puts("0097bb30_TEST PASS");return 0;
}