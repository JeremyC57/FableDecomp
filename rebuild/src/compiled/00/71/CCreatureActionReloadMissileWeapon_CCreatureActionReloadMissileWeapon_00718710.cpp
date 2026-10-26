// CCreatureAction_ReloadMissileWeapon::CCreatureAction_ReloadMissileWeapon(CThingCreatureBase*) -- base CCreatureActionBase(owner, -1.0, 0.0, the owner's random loaded "PROJECTILE_WEAPON_LOAD" animation, zeroed CSoundPair, 0, 0), own vtable (0x012656A4), PAmmo default-constructed
// Label: the manifest row says CCreatureAction_AimMissileWeapon (propagated); the stored vtable 0x012656A4 is CCreatureAction_ReloadMissileWeapon (rtti-vtables.tsv).
// Leaf: ??0CCreatureAction_ReloadMissileWeapon
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00718710, length 123 bytes (+ int3 padding).
// Callees: CCharString::CCharString(const char*, long) @ 0x0099EBF0, the random-loaded-animation lookup @ 0x006924B0
// (`__fastcall`: ecx owner, edx name; a thunk onto CThingCreatureBase::GetRandomLoadedAnimFromName(name, true) @ 0x00662FA0),
// CCreatureActionBase::CCreatureActionBase(CThingCreatureBase*, float, float, CAnimationEntry*, const CSoundPair&, long, long)
// @ 0x00693B30, CCharString::~CCharString @ 0x0099EAE0, CBaseIntelligentPointer::CBaseIntelligentPointer @ 0x00A01B10.
// Shape: the animation name is an IMPLICIT conversion temporary (literal -> CCharString through the defaulted length), so
// it is built first (`push -1 ; push offset`) and re-addressed with `lea` for the lookup; the base arguments go right to
// left (0, 0, the zeroed sound pair, the lookup result, 0.0f as `push 0`, -1.0f, the owner); the temporary dies after
// the base constructor, then the vtable and the CIntelligentPointer member at +0xa8 (base ctor + its vtable 0x0125A858).
// Layout: retail CCreatureActionBase is 0xa8 bytes (no engine header exists for this class; the member mirrors the sibling CCreatureAction_AimMissileWeapon::PAmmo),
// so `this` is modelled locally with PAmmo at +0xa8.
class CThingCreatureBase;
class CAnimationEntry;
class CBaseObject;

class CCharString { public:                        // one pointer; the defaulted length lets the literal convert implicitly
    void* m_pData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                         // 0x0099EAE0
};

class CSoundPair { public:                         // two sound ids, zeroed by the inline constructor (frame stores before the base call)
    long FirstSound;
    long LastSound;
    CSoundPair() : FirstSound(0), LastSound(0) { }
};

CAnimationEntry* __fastcall GetRandomLoadedAnimation(CThingCreatureBase* owner, const CCharString& name);   // 0x006924B0

struct CBaseIntelligentPointer {
    virtual void v00();
    CBaseIntelligentPointer();              // 0x00A01B10
};

struct CIntelligentPointer : CBaseIntelligentPointer {   // CIntelligentPointer<CThingObject>, vtable 0x0125A858
    virtual void v00();
    CIntelligentPointer() : CBaseIntelligentPointer() { }
};

class CCreatureActionBase { public:                // retail sizeof 0xa8
    virtual void v00();
    unsigned char _body[0xa8 - 4];
    CCreatureActionBase(CThingCreatureBase* owner, float a, float b, CAnimationEntry* animation, const CSoundPair& sound, long c, long d);   // 0x00693B30
};

class CCreatureAction_ReloadMissileWeapon : public CCreatureActionBase { public:   // vtable 0x012656A4
    CIntelligentPointer PAmmo;              // +0xa8 (PDB +0xb4)
    virtual void v00();
    CCreatureAction_ReloadMissileWeapon(CThingCreatureBase* owner);
};

CCreatureAction_ReloadMissileWeapon::CCreatureAction_ReloadMissileWeapon(CThingCreatureBase* owner)
    : CCreatureActionBase(owner, -1.0f, 0.0f, GetRandomLoadedAnimation(owner, "PROJECTILE_WEAPON_LOAD"), CSoundPair(), 0, 0)
    , PAmmo()
{
}