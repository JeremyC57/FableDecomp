#include <stdio.h>
class CThing{};class CThingCreatureBase:public CThing{};class CThingPlayerCreature:public CThingCreatureBase{};
class CSoundPair{public:long FirstSound,LastSound;CSoundPair():FirstSound(0),LastSound(0){}};
class CBaseObject;class CBaseObjectPointer;class CBaseIntelligentPointer{public:CBaseIntelligentPointer();virtual ~CBaseIntelligentPointer();CBaseObjectPointer* PData;void SetPItem(CBaseObject*);};template<class T>class CIntelligentPointer:public CBaseIntelligentPointer{public:CIntelligentPointer(T* p):CBaseIntelligentPointer(){SetPItem((CBaseObject*)p);}};
class CBaseClass{public:virtual ~CBaseClass(){} };class CBaseObject:public CBaseClass{public:CBaseObjectPointer* IntelligentPointer;};
class CCreatureActionBase:public CBaseObject{public:unsigned char _unmodeled08[0xa8-8];CCreatureActionBase(CThingCreatureBase&,const CSoundPair&,long,long);};
class CCreatureAction_UseLamp:public CCreatureActionBase{public:CIntelligentPointer<CThing> PObjectToUse;CCreatureAction_UseLamp(CThingCreatureBase&,CThing&);};
CCreatureAction_UseLamp::CCreatureAction_UseLamp(CThingCreatureBase& creature,CThing& target):CCreatureActionBase(creature,CSoundPair(),0,0),PObjectToUse(&target){}
static int calls,bad;static CThingCreatureBase* creatureExpected;static void* targetExpected;static CBaseObjectPointer* marker=(CBaseObjectPointer*)1;
CCreatureActionBase::CCreatureActionBase(CThingCreatureBase& c,const CSoundPair& s,long group,long priority){if(calls++ || &c!=creatureExpected || s.FirstSound || s.LastSound || group || priority)bad=1;}
CBaseIntelligentPointer::CBaseIntelligentPointer(){if(calls++!=1)bad=1;PData=0;}void CBaseIntelligentPointer::SetPItem(CBaseObject* p){if(calls++!=2 || (void*)p!=targetExpected)bad=1;PData=marker;}CBaseIntelligentPointer::~CBaseIntelligentPointer(){}
int main(){CThingCreatureBase creature;CThing target;creatureExpected=&creature;targetExpected=&target;CCreatureAction_UseLamp action(creature,target);if(bad || calls!=3 || action.PObjectToUse.PData!=marker)goto fail;puts("008435b0_TEST PASS");return 0;fail:puts("008435b0_TEST FAIL");return 1;}