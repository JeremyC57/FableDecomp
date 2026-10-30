// CCreatureAction_StopEatingAtTable::Clone() const -- copy the action through its original base and concrete type.
// Leaf: ?Clone@CCreatureAction_StopEatingAtTable@@UBEPAVCCreatureActionBase@@XZ
// Identity: proven; own RTTI vtable slot11 Clone plus exact PDB const API returning CCreatureActionBase*; first copied vtable is CCreatureAction_PutDownThingAtTable; field Mode at0xa8 from own transfer and PDB original; genuine implicit copy through CCreatureActionBase helper0062EBC0.
// Layout: generated headers flatten virtual base inheritance and omit copy/Clone APIs. Real original CBaseClass/CBaseObject/CCreatureActionBase/CCombatActionBase chain retained; base168/combat276, exact allocation0xac. The intermediate class member is original enum/bool, copied implicitly.
#ifdef _MSC_VER
void* __cdecl operator new(unsigned int);
#else
#include <stddef.h>
#endif
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0
class CBaseObjectPointer;class CBaseObject:public CBaseClass{public:CBaseObjectPointer* IntelligentPointer;};class CCreatureActionBase:public CBaseObject{public:unsigned char State[0xa8-8];CCreatureActionBase();CCreatureActionBase(const CCreatureActionBase&);virtual CCreatureActionBase* Clone()const=0;};class CCombatActionBase:public CCreatureActionBase{public:unsigned char CombatState[0x114-0xa8];CCombatActionBase();CCombatActionBase(const CCombatActionBase&);};
enum EUnsheatheWeaponType{UNSHEATHE_DEFAULT=0,UNSHEATHE_TEST=17};namespace NCreatureMode{enum EMode{MODE_DEFAULT=0,MODE_TEST=39};}
class CCreatureAction_PutDownThingAtTable:public CCreatureActionBase{public:NCreatureMode::EMode Mode;};
class CCreatureAction_StopEatingAtTable:public CCreatureAction_PutDownThingAtTable{public:virtual CCreatureActionBase* Clone()const;};
CCreatureActionBase* CCreatureAction_StopEatingAtTable::Clone()const{return new CCreatureAction_StopEatingAtTable(*this);}