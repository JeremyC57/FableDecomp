// 0076ef40 CTCWeaponTrail::OnCreate
// Leaf: ?OnCreate@CTCWeaponTrail@@EAEXXZ
// Flags:
#define CClass CClassFields
#include "engine/CClass.h"
#undef CClass
class CClass:public CClassFields {public:unsigned long GetType()const{return Type;}};
#define CThing CThingFields
#include "engine/_quarantine/CThing.h"
#undef CThing
#define CTCBase CTCBaseFields
#include "engine/CTCBase.h"
#undef CTCBase
#include "engine/CBaseClassNonCopyable.h"
enum ETCInterfaceType {};
// Layout: the shared CThing field projection has a flattened vptr and class
// prefix; this row reads CClass::Type10, UsingComponents20 and ThingTypeFlags6c.
class CThing:public CThingFields {
public:
 bool HasTC(ETCInterfaceType t)const{return (reinterpret_cast<const unsigned long*>(UsingComponents)[(unsigned)t/32]&(1UL<<((unsigned)t%32)))!=0;}
 unsigned long GetTypeFlags()const{return ThingTypeFlags;}
};
class CPersistContext;class CTCParamsBase;
// Layout: shared CTCBase has a flattened vptr. Original CBaseClassNonCopyable
// base and virtual prefix are restored; ParentThing reference4, flags8/9.
class CTCBase:public CBaseClassNonCopyable {
protected:virtual void OnSerialise(CPersistContext&,bool);
public:virtual void OnRemove();virtual void ConstructFromParams(const CTCParamsBase&);virtual void OnCreate();
protected:CThing&ParentThing;
private:bool ToBeRemoved,IsInGlobalUpdateTCs;
};
class CEnginePrimitiveWeaponTrail {public:enum EWeaponTrailType {};};
// Layout: original private fields NoTrails0c, NoProjectileTrails10,
// TrailType2c, MinProjectileTrailTimer64. Vector/list donor shrink is verified
// against this native leaf and constructor; the remaining state is unread.
class CTCWeaponTrail:public CTCBase {
 long NoTrails,NoProjectileTrails;
 unsigned char unreadBeforeType[0x2c-0x14];
 CEnginePrimitiveWeaponTrail::EWeaponTrailType TrailType;
 unsigned char unreadBeforeTimer[0x64-0x30];
 long MinProjectileTrailTimer;
 static bool WeaponTrailAlphaAddedToConsole;
 void InitialiseTrails();
 virtual void OnCreate();
};
void CTCWeaponTrail::OnCreate(){
 if(!WeaponTrailAlphaAddedToConsole)WeaponTrailAlphaAddedToConsole=true;
 if(reinterpret_cast<const CClass*>(&ParentThing)->GetType()==5)
  TrailType=static_cast<CEnginePrimitiveWeaponTrail::EWeaponTrailType>(ParentThing.HasTC(static_cast<ETCInterfaceType>(125))?3:0);
 else if(static_cast<unsigned char>(ParentThing.GetTypeFlags())&8)TrailType=static_cast<CEnginePrimitiveWeaponTrail::EWeaponTrailType>(2);else TrailType=static_cast<CEnginePrimitiveWeaponTrail::EWeaponTrailType>(0);
 NoTrails=-1;NoProjectileTrails=-1;
 InitialiseTrails();MinProjectileTrailTimer=-1;
}
FABLE_STATIC_ASSERT(sizeof(CTCBase)==sizeof(CTCBaseFields));