#include <stdio.h>
#include <string.h>
// CCreatureAction_SheatheWeapons::CCreatureAction_SheatheWeapons(CThingCreatureBase& thing) -- Initialise the combat action and its flag.
// Leaf: ??0CCreatureAction_SheatheWeapons@@QAE@AAVCThingCreatureBase@@@Z
// Identity: own full RTTI 01282cec and original PDB constructor; base00858030 takes CThingCreatureBase&,long,const CSoundPair&.
// Layout: CCombatActionBase header is quarantined. Retail base is0x114 bytes; derived bool+0x114, consistent with donor0x12c after container correction. Opaque base data is not accessed here. The genuine polymorphic model is required for constructor vtable stores; the available derived header is flattened.
class CThingCreatureBase;
class CSoundPair {public:long FirstSound,LastSound;CSoundPair(long first=0,long last=0):FirstSound(first),LastSound(last){}};
class CCombatActionBase {public:virtual ~CCombatActionBase();unsigned char BaseState[0x110];CCombatActionBase(CThingCreatureBase&,long,const CSoundPair&);};
class CCreatureAction_SheatheWeapons:public CCombatActionBase {public:bool PlayedSound;virtual ~CCreatureAction_SheatheWeapons();CCreatureAction_SheatheWeapons(CThingCreatureBase&);};
CCreatureAction_SheatheWeapons::CCreatureAction_SheatheWeapons(CThingCreatureBase& thing):CCombatActionBase(thing,0,CSoundPair()),PlayedSound(false){}

class CThingCreatureBase {};
static CThingCreatureBase* expected;static int bases,dtors,bad;
CCombatActionBase::CCombatActionBase(CThingCreatureBase& thing,long priority,const CSoundPair& sounds){++bases;if(&thing!=expected || priority!=0 || sounds.FirstSound || sounds.LastSound)bad=1;memset(BaseState,0x5a,sizeof(BaseState));}
CCombatActionBase::~CCombatActionBase(){++dtors;}
CCreatureAction_SheatheWeapons::~CCreatureAction_SheatheWeapons(){}
int main(){CThingCreatureBase thing;expected=&thing;int ok;
{CCreatureAction_SheatheWeapons action(thing);ok=!bad && bases==1 && !dtors && !action.PlayedSound;
for(unsigned int i=0;i<sizeof(action.BaseState);++i)ok=ok && action.BaseState[i]==0x5a;}
ok=ok && dtors==1;puts(ok?"008cc370_TEST PASS":"008cc370_TEST FAIL");return ok?0:1;}