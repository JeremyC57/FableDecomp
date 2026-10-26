// CCreatureAction_HobbeSpellcasterFire::PerformAction() -- fires the projectile through the base PerformAction, then attaches the "FIRE_EFFECT" to the creature's staff
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008C9C80, length 64 bytes. Vtable slot 3 (PerformAction).
// Callees: CCreatureAction_GenericFireProjectileWeapon::PerformAction @ 0x008BF280 (direct base call), CCharString::CCharString
// (const char*, long) @ 0x0099EBF0, CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (PCreature at +8), AttachEffectToStaff
// @ 0x008C9A40 (__fastcall free function: creature in ecx, name in edx; manifest name), CCharString::~CCharString @ 0x0099EAE0.
// Shape: the literal converts implicitly to a CCharString temporary (built first -- arguments evaluate right to left -- and
// re-addressed with `lea edx` after the GetPItem call, the implicit-conversion signature from GOTCHAS), then destroyed.
// Retail CCreatureActionBase is 0xa8 bytes (PDB 0xb4), so `this` is modelled locally.
class CThingCreatureBase;
class CThing;

class CCharString { public:                        // one pointer; the default length makes the literal convert implicitly
    void* m_pData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                         // 0x0099EAE0
};

CThing* __fastcall AttachEffectToStaff(CThingCreatureBase* creature, const CCharString& effectName);   // 0x008C9A40

class CBaseObject;
class CBaseObjectPointer;
class CBaseIntelligentPointer {                       // vtable 0x0129C95C (Ego_r: vptr + PData, 8 bytes)
public:
    CBaseIntelligentPointer();                           // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                  // 0x00A01C10
    CBaseObject* GetPItem() const;                       // 0x00A01B50
    void SetPItem(CBaseObject* item);                    // 0x00A01B90
private:
    CBaseObjectPointer* PData;                           // +0x04
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
    void SetPItem(T* item) { CBaseIntelligentPointer::SetPItem((CBaseObject*)item); }
};

class CCreatureAction_GenericFireProjectileWeapon { public:   // engine/CCreatureAction_GenericFireProjectileWeapon.h exists but is not used: retail CCreatureActionBase is 0xa8 bytes, the PDB build's 0xb4
    virtual void v00(); virtual void v01(); virtual void v02();
    virtual void PerformAction();           // slot 3, 0x008BF280
    unsigned char _pad04[4];                // +0x04
    CIntelligentPointer<CThingCreatureBase> PCreature;      // +0x08 CIntelligentPointer<CThingCreatureBase>
};

class CCreatureAction_HobbeSpellcasterFire : public CCreatureAction_GenericFireProjectileWeapon { public:
    virtual void PerformAction();
};

void CCreatureAction_HobbeSpellcasterFire::PerformAction()
{
    CCreatureAction_GenericFireProjectileWeapon::PerformAction();
    AttachEffectToStaff(PCreature.GetPItem(), "FIRE_EFFECT");
}