#ifdef _MSC_VER
#pragma optimize("s",on)
#endif
#include <stdio.h>
class CBaseClass{public:virtual ~CBaseClass(){} };class CAIBrain;class CCharStringData;class CCharString{public:CCharStringData* PStringData;};enum EScriptAIPriority{SCRIPT_PRIORITY_DEFAULT};
class CAIStateGroupBase:public CBaseClass{public:CAIBrain* PBrain;EScriptAIPriority ScriptAIPriority;CCharString Name;bool Terminated;unsigned char Priority;bool Enabled;CAIStateGroupBase();};template<class T>class CAIStateGroup_WorkBase:public T{};
class CAIStateGroup_JackOfBladesBase:public CAIStateGroupBase{public:unsigned char _unmodeled14[0x4c-0x14];CAIStateGroup_JackOfBladesBase();};
class CBaseObject;class CBaseObjectPointer;class CThing;class CThingCreatureBase;class CBaseIntelligentPointer{public:CBaseIntelligentPointer();virtual ~CBaseIntelligentPointer();CBaseObjectPointer* PData;};template<class T>class CIntelligentPointer:public CBaseIntelligentPointer{public:CIntelligentPointer():CBaseIntelligentPointer(){}virtual ~CIntelligentPointer(){}};
class CAIStateGroup_BalverineCornered:public CAIStateGroupBase{public: CIntelligentPointer<CThingCreatureBase> PTarget,PLastJumpTarget;CAIStateGroup_BalverineCornered();};
CAIStateGroup_BalverineCornered::CAIStateGroup_BalverineCornered(){}
static int calls,baseCalls;static CBaseIntelligentPointer* members[2];CBaseIntelligentPointer::CBaseIntelligentPointer():PData(0){if(calls<2)members[calls]=this;++calls;}CBaseIntelligentPointer::~CBaseIntelligentPointer(){}CAIStateGroupBase::CAIStateGroupBase(){++baseCalls;}CAIStateGroup_JackOfBladesBase::CAIStateGroup_JackOfBladesBase(){}
int main(){CAIStateGroup_BalverineCornered group;if(calls!=2 || baseCalls!=1 || members[0]!=&group.PTarget || members[1]!=&group.PLastJumpTarget || group.PTarget.PData || group.PLastJumpTarget.PData){puts("009004e0_TEST FAIL");return 1;}puts("009004e0_TEST PASS");return 0;}