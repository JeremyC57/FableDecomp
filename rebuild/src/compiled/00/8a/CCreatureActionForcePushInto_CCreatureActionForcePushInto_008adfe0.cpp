// CCreatureAction_ForcePushInto::CCreatureAction_ForcePushInto(CThingCreatureBase& thing) -- Initialise the combat action and its flag.
// Leaf: ??0CCreatureAction_ForcePushInto@@QAE@AAVCThingCreatureBase@@@Z
// Identity: own full RTTI 012783d4 and original PDB constructor; base00858030 takes CThingCreatureBase&,long,const CSoundPair&.
// Layout: CCombatActionBase header is quarantined. Retail base is0x114 bytes; derived bool+0x114, consistent with donor0x12c after container correction. Opaque base data is not accessed here. The genuine polymorphic model is required for constructor vtable stores; the available derived header is flattened.
class CThingCreatureBase;
class CSoundPair {public:long FirstSound,LastSound;CSoundPair(long first=0,long last=0):FirstSound(first),LastSound(last){}};
class CCombatActionBase {public:virtual ~CCombatActionBase();unsigned char BaseState[0x110];CCombatActionBase(CThingCreatureBase&,long,const CSoundPair&);};
class CCreatureAction_ForcePushInto:public CCombatActionBase {public:bool OnlyHitEnemies;virtual ~CCreatureAction_ForcePushInto();CCreatureAction_ForcePushInto(CThingCreatureBase&);};
CCreatureAction_ForcePushInto::CCreatureAction_ForcePushInto(CThingCreatureBase& thing):CCombatActionBase(thing,150,CSoundPair()),OnlyHitEnemies(false){}