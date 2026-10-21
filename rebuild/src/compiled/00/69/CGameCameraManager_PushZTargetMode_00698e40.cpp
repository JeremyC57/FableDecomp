// CGameCameraManager::PushZTargetMode() -- block mode changes and mark the camera as manually driven; when the observed thing is alive and no camera script is pending or running, play the Z_TARGET_START sound, fetch the manager def's Z-target camera mode def and prepare it, switching to the pending mode when one was built
// Retry 10b (getsw sweep, FIXES B-15): the deleted flag is read through an inline getter `GetDeleted()`; the plain member read gives the parked register/scheduling difference.
// Leaf: PushZTargetMode
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00698E40, length 269 bytes (+ int3 padding), plain `ret`.
// Signature from Ego_r.pdb: ?PushZTargetMode@CGameCameraManager@@QAEXXZ.
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50, the script's IsFinished (virtual slot 2), CCharString(const char*,
//   long) @ 0x0099EBF0 ("Z_TARGET_START" @ 0x0125C334) / ~CCharString @ 0x0099EAE0, PlayCameraSoundThroughObservedThing(const
//   CThing*, const CCameraManagerDef&, const CCharString&) @ 0x00698A30 (__fastcall), CDefinitionManager::GetDef<CCameraModeDef>
//   @ 0x0056D0C0, CGameCameraManager::PreparePendingMode(CDefPointer<const CCameraModeDef>) @ 0x00698840 and SetCameraMode(
//   CCountedPointer<NCameraModes::CBase>) @ 0x00697E20, the counted pointer's copy ctor @ 0x0056C050, the def's slot 1 on release.
// Layout: the landed PushSpecialCameraMode 0x00698F50 / PushProjectileWeaponMode 0x00698D10 model (engine/CGameCameraManager.h
//   carries contradictory PDB offsets): World +0x74, PendingMode +0xa0, CurrentScript +0xd4, PendingScript +0xdc, Observed
//   +0x100, UsingCameraManually +0x12c, BlockModeChange +0x12d, PDef +0x134; CCameraManagerDef ZTargetMode +0x58; the world's
//   definition manager +0x10.
// Shape: a same-TU `auto_inline(off)` stand-in for GetDef 0x0056D0C0 (precedent 0x00848730) keeps the def in edi and lets
//   it share the name's frame word; DecRefToZero gives the short release. The PushSpecialCameraMode prelude (the constant 1 kept in bl for the two flag stores and the dead-bit test); the
//   sound name in its own block; the mode def pointer shares the name's frame word, is passed by value (copy built in the
//   argument slot, add-ref through a null test) and released at the end through the held register (`dec [edi+4] ; jne`).
struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
static __forceinline bool DecRefToZero(long& count) { return --count == 0; }
struct CCameraModeDef {
    virtual void v00();
    virtual void Destroy();                             // slot 1
    long IVRefCount;                                    // +0x04
};
struct CCameraDefPointer {
    CCameraModeDef* Object;
    CCameraDefPointer() : Object(0) {}
    CCameraDefPointer(const CCameraDefPointer& p) : Object(p.Object) { if (Object) ++Object->IVRefCount; }
    ~CCameraDefPointer() { if (Object) { if (DecRefToZero(Object->IVRefCount)) Object->Destroy(); } }
    CCameraDefPointer& operator=(const CCameraDefPointer& other)
    {
        if (Object != other.Object) {
            if (Object) {
                if (--Object->IVRefCount == 0)
                    Object->Destroy();
                Object = 0;
            }
            Object = other.Object;
            if (Object)
                ++Object->IVRefCount;
        }
        return *this;
    }
};
struct CCameraManagerDef {
    unsigned char _pad00[0x54];
    long InitialMode;                                   // +0x54
    long ZTargetMode;                                   // +0x58
    long ProjectileWeaponMode;                          // +0x5c
};
struct CGameDefinitionManager {
    CCameraDefPointer GetPDefFromGlobalIndex(long index);   // 0x009AD9E0 (hidden result pointer)
    bool GetDef(long globalIndex, CCameraDefPointer* out);   // 0x0056D0C0
};
#pragma auto_inline(off)
// Same-TU stand-in for CDefinitionManager::GetDef<CCameraModeDef> 0x0056D0C0 (the landed 0x0042AF3C body of the family):
// compiled above the caller so VC7.1 knows the def pointer's address does not escape (retail's LTCG knowledge).
bool CGameDefinitionManager::GetDef(long globalIndex, CCameraDefPointer* out)
{
    if (globalIndex <= 0)
        return false;
    CCameraDefPointer def = GetPDefFromGlobalIndex(globalIndex);
    if (def.Object) {
        *out = def;
        return true;
    }
    return false;
}
#pragma auto_inline(on)
struct CWorld {
    unsigned char _pad00[0x10];
    CGameDefinitionManager* DefinitionManager;          // +0x10
};
struct CCameraMode { void* Vtable; };
struct CCameraModeOwner;
struct CCameraModePointer {
    CCameraMode*      Data;
    CCameraModeOwner* Info;
    CCameraModePointer(const CCameraModePointer& other);   // 0x0056C050
    ~CCameraModePointer();
};
struct CCameraScript {
    virtual void v00();
    virtual void v01();
    virtual bool IsFinished() const;                    // slot 2
};
struct CCameraScriptPointer { CCameraScript* Data; CCameraModeOwner* Info; };
struct CThing {
    unsigned char _pad00[0x91];
    unsigned char Deleted;                              // +0x91 bit 0
    unsigned char GetDeleted() const { return Deleted; }
};
struct CCameraObservedPointer {
    void* Vtable;
    void* Item;
    const CThing* GetPItem() const;                     // 0x00A01B50
};
void __fastcall PlayCameraSoundThroughObservedThing(const CThing* thing, const CCameraManagerDef& def, const CCharString& sound);   // 0x00698A30

struct CGameCameraManager {
    unsigned char          _pad00[0x74];
    CWorld*                World;                       // +0x74
    unsigned char          CamTools[0x20];              // +0x78
    CCameraModePointer     CurrentMode;                 // +0x98
    CCameraModePointer     PendingMode;                 // +0xa0
    unsigned char          _pada8[0x2c];
    CCameraScriptPointer   CurrentScript;               // +0xd4
    CCameraScriptPointer   PendingScript;               // +0xdc
    unsigned char          _pade4[0x1c];
    CCameraObservedPointer Observed;                    // +0x100
    unsigned char          _pad108[0x24];
    bool                   UsingCameraManually;         // +0x12c
    bool                   BlockModeChange;             // +0x12d
    unsigned char          _pad12e[2];
    void*                  ManagerSet;                  // +0x130
    CCameraManagerDef*     PDef;                        // +0x134
    void PreparePendingMode(CCameraDefPointer mode);    // 0x00698840
    void SetCameraMode(CCameraModePointer mode);        // 0x00697E20
    void PushZTargetMode();
};

void CGameCameraManager::PushZTargetMode()
{
    BlockModeChange = true;
    UsingCameraManually = true;
    if (Observed.GetPItem() && !(Observed.GetPItem()->GetDeleted() & 1) && !PendingScript.Data &&
        (!CurrentScript.Data || CurrentScript.Data->IsFinished())) {
        {
            CCharString sound("Z_TARGET_START", -1);
            const CCameraManagerDef& def = *PDef;
            PlayCameraSoundThroughObservedThing(Observed.GetPItem(), def, sound);
        }
        CCameraDefPointer mode;
        if (PDef && PDef->ZTargetMode) {
            World->DefinitionManager->GetDef(PDef->ZTargetMode, &mode);
            PreparePendingMode(mode);
            if (PendingMode.Data)
                SetCameraMode(PendingMode);
        }
    }
}