#ifdef _MSC_VER
#pragma optimize("s",on)
#endif
#include <stdio.h>
class CBaseClass{public:virtual ~CBaseClass(){} };class CAIBrain;class CCharStringData;class CCharString{public:CCharStringData* PStringData;};enum EScriptAIPriority{SCRIPT_PRIORITY_DEFAULT};
class CAIStateGroupBase:public CBaseClass{public:CAIBrain* PBrain;EScriptAIPriority ScriptAIPriority;CCharString Name;bool Terminated;unsigned char Priority;bool Enabled;CAIStateGroupBase();};template<class T>class CAIStateGroup_WorkBase:public T{};
class CAIStateGroup_JackOfBladesBase:public CAIStateGroupBase{public:unsigned char _unmodeled14[0x4c-0x14];CAIStateGroup_JackOfBladesBase();};
class CBaseObject;class CBaseObjectPointer;class CThing;class CThingCreatureBase;class CBaseIntelligentPointer{public:CBaseIntelligentPointer();virtual ~CBaseIntelligentPointer();CBaseObjectPointer* PData;};template<class T>class CIntelligentPointer:public CBaseIntelligentPointer{public:CIntelligentPointer():CBaseIntelligentPointer(){}virtual ~CIntelligentPointer(){}};
class CAIStateGroup_WanderAroundShops:public CAIStateGroup_WorkBase<CAIStateGroupBase>{public: CIntelligentPointer<CThing> Shop,PrettyThing;CAIStateGroup_WanderAroundShops();};
CAIStateGroup_WanderAroundShops::CAIStateGroup_WanderAroundShops(){}
static int calls,baseCalls;static CBaseIntelligentPointer* members[2];CBaseIntelligentPointer::CBaseIntelligentPointer():PData(0){if(calls<2)members[calls]=this;++calls;}CBaseIntelligentPointer::~CBaseIntelligentPointer(){}CAIStateGroupBase::CAIStateGroupBase(){++baseCalls;}CAIStateGroup_JackOfBladesBase::CAIStateGroup_JackOfBladesBase(){}
int main(){CAIStateGroup_WanderAroundShops group;if(calls!=2 || baseCalls!=1 || members[0]!=&group.Shop || members[1]!=&group.PrettyThing || group.Shop.PData || group.PrettyThing.PData){puts("009000bc_TEST FAIL");return 1;}puts("009000bc_TEST PASS");return 0;}