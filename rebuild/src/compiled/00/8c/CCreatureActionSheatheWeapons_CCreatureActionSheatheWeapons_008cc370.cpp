// CCreatureAction_SheatheWeapons::CCreatureAction_SheatheWeapons(CThingCreatureBase& thing) -- Initialise the combat action and its flag.
// Leaf: ??0CCreatureAction_SheatheWeapons@@QAE@AAVCThingCreatureBase@@@Z
// Identity: own full RTTI 01282cec and original PDB constructor; base00858030 takes CThingCreatureBase&,long,const CSoundPair&.
// Layout: CCombatActionBase header is quarantined. Retail base is0x114 bytes; derived bool+0x114, consistent with donor0x12c after container correction. Opaque base data is not accessed here. The genuine polymorphic model is required for constructor vtable stores; the available derived header is flattened.
class CThingCreatureBase;
class CSoundPair {public:long FirstSound,LastSound;CSoundPair(long first=0,long last=0):FirstSound(first),LastSound(last){}};
class CCombatActionBase {public:virtual ~CCombatActionBase();unsigned char BaseState[0x110];CCombatActionBase(CThingCreatureBase&,long,const CSoundPair&);};
class CCreatureAction_SheatheWeapons:public CCombatActionBase {public:bool PlayedSound;virtual ~CCreatureAction_SheatheWeapons();CCreatureAction_SheatheWeapons(CThingCreatureBase&);};
CCreatureAction_SheatheWeapons::CCreatureAction_SheatheWeapons(CThingCreatureBase& thing):CCombatActionBase(thing,0,CSoundPair()),PlayedSound(false){}