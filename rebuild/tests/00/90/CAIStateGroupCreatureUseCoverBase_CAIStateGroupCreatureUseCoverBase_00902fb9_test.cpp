#include <stdio.h>
class CThing; class CThingCreatureBase;
class CBaseIntelligentPointer { public: virtual ~CBaseIntelligentPointer(); CBaseIntelligentPointer(); void* PData; };
template<class T> class CIntelligentPointer:public CBaseIntelligentPointer { public: CIntelligentPointer():CBaseIntelligentPointer() {} };
class CEnginePrimitiveThemeChannelDesc { public: unsigned char ChannelIndex0,ChannelIndex1,ChannelBlendFactor,Pad; CEnginePrimitiveThemeChannelDesc():ChannelIndex0(0),ChannelIndex1(0),ChannelBlendFactor(0) {} };
namespace std {
template<class T> class allocator {};
template<class V,class T,class Alloc> class _STLP_alloc_proxy:public Alloc { public: V _M_data; };
template<class T,class Alloc=allocator<T> > class vector { public: T* _M_start; T* _M_finish; _STLP_alloc_proxy<T*,T,Alloc> _M_end_of_storage; vector():_M_start(0),_M_finish(0) { _M_end_of_storage._M_data=0; } };
}
class CAIBrain; class CCharString { public: void* PStringData; }; enum EScriptAIPriority {};
class CAIStateGroupBase { public: virtual ~CAIStateGroupBase() {} CAIBrain* PBrain; EScriptAIPriority ScriptAIPriority; CCharString Name; bool Terminated; unsigned char Priority; bool Enabled; };
class CAIStateGroup_EnemyCreatureBase:public CAIStateGroupBase { public: CAIStateGroup_EnemyCreatureBase(); CIntelligentPointer<CThingCreatureBase> PTargetThing; };
#pragma pack(push,1)
class CAIStateGroup_CreatureUseCoverBase:public CAIStateGroup_EnemyCreatureBase { public: bool HasDiveAnim,HasJumpOutAnims,TimingCritical; CIntelligentPointer<CThing> PCoverThing; unsigned char _pad27; std::vector<CIntelligentPointer<CThing> > CoverList; CAIStateGroup_CreatureUseCoverBase(); };
#pragma pack(pop)
CAIStateGroup_CreatureUseCoverBase::CAIStateGroup_CreatureUseCoverBase():CAIStateGroup_EnemyCreatureBase() {}
static int baseCalls,ptrCalls;
CBaseIntelligentPointer::CBaseIntelligentPointer() { ++ptrCalls; PData=0; }
CBaseIntelligentPointer::~CBaseIntelligentPointer() {}
CAIStateGroup_EnemyCreatureBase::CAIStateGroup_EnemyCreatureBase() { ++baseCalls; }
int main() { CAIStateGroup_CreatureUseCoverBase object; if(baseCalls!=1 || ptrCalls!=2 || object.PCoverThing.PData || object.CoverList._M_start || object.CoverList._M_finish || object.CoverList._M_end_of_storage._M_data) { puts("00902fb9_TEST FAIL"); return 1; } puts("00902fb9_TEST PASS"); return 0; }