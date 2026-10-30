// CCreatureAction_UseWallMount::CCreatureAction_UseWallMount(CThingCreatureBase& creature, CThing& target) -- initialize the action and track its target.
// Leaf: ??0CCreatureAction_UseWallMount@@QAE@AAVCThingCreatureBase@@AAVCThing@@@Z
// Identity: proven; own RTTI vtable0x0127498c and PDB two-reference constructor; base CCreatureActionBase(Creature&,CSoundPair,0,0)00693C70; tracked member+0xA8 calls base ctor00A01B10 and SetPItem00A01B90.
// Layout: available action headers flatten polymorphic bases and tracked pointers into data, so cannot express real ctor/vtable chain; local original CBaseClass/CBaseObject8 + action168, RTTI confirms action -> CBaseObject -> CBaseClass + actual tracked member8. Shared CSoundPair header absent; PDB default ctor sets both indices0.
class CThing{};class CThingCreatureBase:public CThing{};class CThingPlayerCreature:public CThingCreatureBase{};
class CSoundPair{public:long FirstSound,LastSound;CSoundPair():FirstSound(0),LastSound(0){}};
class CBaseObject;class CBaseObjectPointer;class CBaseIntelligentPointer{public:CBaseIntelligentPointer();virtual ~CBaseIntelligentPointer();CBaseObjectPointer* PData;void SetPItem(CBaseObject*);};template<class T>class CIntelligentPointer:public CBaseIntelligentPointer{public:CIntelligentPointer(T* p):CBaseIntelligentPointer(){SetPItem((CBaseObject*)p);}};
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0
#define FABLE_RETAIL_BASE_OBJECT
#include "engine/CBaseObject.h"                       // : CBaseClass, vtable 0x0129A7D8 (~ slot 0, Validate slot 1)
class CCreatureActionBase:public CBaseObject{public:unsigned char _unmodeled08[0xa8-8];CCreatureActionBase(CThingCreatureBase&,const CSoundPair&,long,long);};
class CCreatureAction_UseWallMount:public CCreatureActionBase{public:CIntelligentPointer<CThing> PObjectToUse;CCreatureAction_UseWallMount(CThingCreatureBase&,CThing&);};
CCreatureAction_UseWallMount::CCreatureAction_UseWallMount(CThingCreatureBase& creature,CThing& target):CCreatureActionBase(creature,CSoundPair(),0,0),PObjectToUse(&target){}