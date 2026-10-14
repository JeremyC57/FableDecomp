// CCreatureAction_HobbeSpellcasterFire::PerformAction() -- fires the projectile through the base PerformAction, then attaches the "FIRE_EFFECT" to the creature's staff
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008C9C80, length 64 bytes. Vtable slot 3 (PerformAction).
// Callees: CCreatureAction_GenericFireProjectileWeapon::PerformAction @ 0x008BF280 (direct base call), CCharString::CCharString
// (const char*, long) @ 0x0099EBF0, CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (PCreature at +8), AttachEffectToStaff
// @ 0x008C9A40 (__fastcall free function: creature in ecx, name in edx; manifest name), CCharString::~CCharString @ 0x0099EAE0.
// Shape: the literal converts implicitly to a CCharString temporary (built first -- arguments evaluate right to left -- and
// re-addressed with `lea edx` after the GetPItem call, the implicit-conversion signature from GOTCHAS), then destroyed.
// Retail CCreatureActionBase is 0xa8 bytes (PDB 0xb4), so `this` is modelled locally.
struct CThingCreatureBase;
struct CThing;

struct CCharString {                        // one pointer; the default length makes the literal convert implicitly
    void* m_pData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                         // 0x0099EAE0
};

CThing* __fastcall AttachEffectToStaff(CThingCreatureBase* creature, const CCharString& effectName);   // 0x008C9A40

struct CBaseIntelligentPointer {
    virtual void v00();
    CThingCreatureBase* m_pItem;            // +0x04
    CThingCreatureBase* GetPItem() const;   // 0x00A01B50
};

struct CCreatureAction_GenericFireProjectileWeapon {   // engine/CCreatureAction_GenericFireProjectileWeapon.h exists but is not used: retail CCreatureActionBase is 0xa8 bytes, the PDB build's 0xb4
    virtual void v00(); virtual void v01(); virtual void v02();
    virtual void PerformAction();           // slot 3, 0x008BF280
    unsigned char _pad04[4];                // +0x04
    CBaseIntelligentPointer PCreature;      // +0x08 CIntelligentPointer<CThingCreatureBase>
};

struct CCreatureAction_HobbeSpellcasterFire : CCreatureAction_GenericFireProjectileWeapon {
    virtual void PerformAction();
};

void CCreatureAction_HobbeSpellcasterFire::PerformAction()
{
    CCreatureAction_GenericFireProjectileWeapon::PerformAction();
    AttachEffectToStaff(PCreature.GetPItem(), "FIRE_EFFECT");
}
