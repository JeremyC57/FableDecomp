// CCreatureAction_SummonerSheatheWeapon::Clone() const -- copy the action through its original base and concrete type.
// Leaf: ?Clone@CCreatureAction_SummonerSheatheWeapon@@UBEPAVCCreatureActionBase@@XZ
// Identity: proven; own RTTI vtable slot11 Clone plus exact PDB const API returning CCreatureActionBase*; first copied vtable is CCreatureAction_SheatheWeapons; field PlayedSound at0x114 from own transfer and PDB original; genuine implicit copy through CCombatActionBase helper0062F3B0.
// Layout: generated headers flatten virtual base inheritance and omit copy/Clone APIs. Real original CBaseClass/CBaseObject/CCreatureActionBase/CCombatActionBase chain retained; base168/combat276, exact allocation0x118. The intermediate class member is original enum/bool, copied implicitly.
#ifdef _MSC_VER
void* __cdecl operator new(unsigned int);
#else
#include <stddef.h>
#endif
class CBaseClass{public:virtual ~CBaseClass(){} };class CBaseObjectPointer;class CBaseObject:public CBaseClass{public:CBaseObjectPointer* IntelligentPointer;};class CCreatureActionBase:public CBaseObject{public:unsigned char State[0xa8-8];CCreatureActionBase();CCreatureActionBase(const CCreatureActionBase&);virtual CCreatureActionBase* Clone()const=0;};class CCombatActionBase:public CCreatureActionBase{public:unsigned char CombatState[0x114-0xa8];CCombatActionBase();CCombatActionBase(const CCombatActionBase&);};
enum EUnsheatheWeaponType{UNSHEATHE_DEFAULT=0,UNSHEATHE_TEST=17};namespace NCreatureMode{enum EMode{MODE_DEFAULT=0,MODE_TEST=39};}
class CCreatureAction_SheatheWeapons:public CCombatActionBase{public:bool PlayedSound;};
class CCreatureAction_SummonerSheatheWeapon:public CCreatureAction_SheatheWeapons{public:virtual CCreatureActionBase* Clone()const;};
CCreatureActionBase* CCreatureAction_SummonerSheatheWeapon::Clone()const{return new CCreatureAction_SummonerSheatheWeapon(*this);}