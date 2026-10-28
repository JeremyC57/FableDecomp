// CCreatureAction_UseLamp::CCreatureAction_UseLamp(CThingCreatureBase& creature, CThing& target) -- initialize the action and track its target.
// Leaf: ??0CCreatureAction_UseLamp@@QAE@AAVCThingCreatureBase@@AAVCThing@@@Z
// Identity: proven; own RTTI vtable0x0127486c and PDB two-reference constructor; base CCreatureActionBase(Creature&,CSoundPair,0,0)00693C70; tracked member+0xA8 calls base ctor00A01B10 and SetPItem00A01B90.
// Layout: available action headers flatten polymorphic bases and tracked pointers into data, so cannot express real ctor/vtable chain; local original CBaseClass/CBaseObject8 + action168, RTTI confirms action -> CBaseObject -> CBaseClass + actual tracked member8. Shared CSoundPair header absent; PDB default ctor sets both indices0.
class CThing{};class CThingCreatureBase:public CThing{};class CThingPlayerCreature:public CThingCreatureBase{};
class CSoundPair{public:long FirstSound,LastSound;CSoundPair():FirstSound(0),LastSound(0){}};
class CBaseObject;class CBaseObjectPointer;class CBaseIntelligentPointer{public:CBaseIntelligentPointer();virtual ~CBaseIntelligentPointer();CBaseObjectPointer* PData;void SetPItem(CBaseObject*);};template<class T>class CIntelligentPointer:public CBaseIntelligentPointer{public:CIntelligentPointer(T* p):CBaseIntelligentPointer(){SetPItem((CBaseObject*)p);}};
class CBaseClass{public:virtual ~CBaseClass(){} };class CBaseObject:public CBaseClass{public:CBaseObjectPointer* IntelligentPointer;};
class CCreatureActionBase:public CBaseObject{public:unsigned char _unmodeled08[0xa8-8];CCreatureActionBase(CThingCreatureBase&,const CSoundPair&,long,long);};
class CCreatureAction_UseLamp:public CCreatureActionBase{public:CIntelligentPointer<CThing> PObjectToUse;CCreatureAction_UseLamp(CThingCreatureBase&,CThing&);};
CCreatureAction_UseLamp::CCreatureAction_UseLamp(CThingCreatureBase& creature,CThing& target):CCreatureActionBase(creature,CSoundPair(),0,0),PObjectToUse(&target){}