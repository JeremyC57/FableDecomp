// CCreatureAction_HeroLoadRangedWeapon::CCreatureAction_HeroLoadRangedWeapon(CThingCreatureBase& creature, CThing& ranged_weapon) -- the base action for the hero: speed -1, end time 1.0f, the hero's random "PROJECTILE_WEAPON_LOAD" animation, an empty sound pair, 0 / 0; then PRangedWeapon = the weapon and PAmmo = null
// Leaf: ??0CCreatureAction_HeroLoadRangedWeapon@@QAE@AAVCThingCreatureBase@@AAVCThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0078AE30, 166 bytes, thiscall, `ret 8`. Vtable 0x0126B9D4.
// Identity (proven): it stores vtable 0x0126B9D4 whose RTTI is .?AVCCreatureAction_HeroLoadRangedWeapon@@; Ego_r.pdb
//   ??0CCreatureAction_HeroLoadRangedWeapon@@QAE@AAVCThingCreatureBase@@AAVCThing@@@Z (donor 0x00791DF0); the PDB members are
//   PRangedWeapon (CIntelligentPointer<CThing>, +0xa8, vtable 0x01238C6C) and PAmmo (CIntelligentPointer<CThingObject>, +0xb0,
//   vtable 0x0125A858); the literal 0x012657BC is "PROJECTILE_WEAPON_LOAD".
// Callees: CCharString::CCharString(const char*, long) 0x0099EBF0, CCreatureActionBase::GetRandomAnimFromName(const
//   CThingCreatureBase&, const CCharString&) 0x006924B0 (`__fastcall`), CCreatureActionBase::CCreatureActionBase(
//   CThingCreatureBase&, float, float, const CAnimationEntry&, const CSoundPair&, long, long) 0x00693B30,
//   CCharString::~CCharString() 0x0099EAE0, CBaseIntelligentPointer::CBaseIntelligentPointer() 0x00A01B10,
//   CBaseIntelligentPointer::SetPItem(CBaseObject*) 0x00A01B90.
// Shape: the landed BanditKingChangeWeapons constructor (0x008C7F60) with end time 1.0f, then two inline
//   CIntelligentPointer<T>(T*) member constructors (base ctor, own vtable, SetPItem).
// Layout: engine/CCreatureAction_HeroLoadRangedWeapon.h (retail sizeof 0xb8, PRangedWeapon +0xa8, PAmmo +0xb0) is a flat pad
//   struct with no bases, so it is included under an alias and the class below (real base + member types) is pinned to it.
#define CCreatureAction_HeroLoadRangedWeapon CCreatureAction_HeroLoadRangedWeaponLayout
#include "engine/CCreatureAction_HeroLoadRangedWeapon.h"
#undef CCreatureAction_HeroLoadRangedWeapon
class CAnimationEntry;
class CThing;
class CThingObject;
class CBaseObject;
class CBaseObjectPointer;
class CCharString {
public:
    CCharString(const char* text, long length = -1);         // 0x0099EBF0
    ~CCharString();                                          // 0x0099EAE0
    void* PStringData;
};
class CSoundPair {
public:
    CSoundPair() : FirstSound(0), LastSound(0) { }
    long FirstSound;
    long LastSound;
};
class CThingCreatureBase {
public:
    virtual ~CThingCreatureBase();
};
class CCreatureActionBase {
public:
    CCreatureActionBase(CThingCreatureBase& creature, float a, float b, const CAnimationEntry& anim, const CSoundPair& sound,
                        long c, long d);                     // 0x00693B30
    virtual ~CCreatureActionBase();
    unsigned char _body[0xa8 - 4];                           // retail sizeof(CCreatureActionBase) 0xA8
protected:
    static const CAnimationEntry& __fastcall GetRandomAnimFromName(const CThingCreatureBase& creature,
                                                                const CCharString& name);   // 0x006924B0
};
class CBaseIntelligentPointer {
public:
    CBaseIntelligentPointer();                               // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                      // 0x00A01C10
    void SetPItem(CBaseObject* item);                        // 0x00A01B90
private:
    CBaseObjectPointer* PData;                               // +0x04
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    CIntelligentPointer(T* item) : CBaseIntelligentPointer() { SetPItem(item); }
    void SetPItem(T* item) { CBaseIntelligentPointer::SetPItem((CBaseObject*)item); }
};
class CCreatureAction_HeroLoadRangedWeapon : public CCreatureActionBase {
public:
    CCreatureAction_HeroLoadRangedWeapon(CThingCreatureBase& creature, CThing& ranged_weapon);
    CIntelligentPointer<CThing> PRangedWeapon;               // +0xa8
    CIntelligentPointer<CThingObject> PAmmo;                 // +0xb0
};

FABLE_STATIC_ASSERT(sizeof(CCreatureAction_HeroLoadRangedWeapon) == sizeof(CCreatureAction_HeroLoadRangedWeaponLayout));

CCreatureAction_HeroLoadRangedWeapon::CCreatureAction_HeroLoadRangedWeapon(CThingCreatureBase& creature, CThing& ranged_weapon)
    : CCreatureActionBase(creature, -1.0f, 1.0f, GetRandomAnimFromName(creature, "PROJECTILE_WEAPON_LOAD"), CSoundPair(), 0, 0)
    , PRangedWeapon(&ranged_weapon)
    , PAmmo(0)
{
}