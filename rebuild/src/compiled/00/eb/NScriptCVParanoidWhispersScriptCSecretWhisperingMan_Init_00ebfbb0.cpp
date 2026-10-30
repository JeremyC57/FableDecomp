// NScript::CV_ParanoidWhispersScript::CSecretWhisperingMan::Init() -- the whispering man is persistent, gets a green minimap marker and remembers his start position
// Leaf: ?Init@CSecretWhisperingMan@CV_ParanoidWhispersScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00EBFBB0, 94 bytes, thiscall, plain `ret` (private virtual, entity vtable slot 2).
// Identity (proven): slot 2 (Init) of the RTTI-less entity vtable 0x012F1010, which the landed
//   NScript::CV_ParanoidWhispersScript::CSecretWhisperingMan::Alloc 0x00EBFB00 stores (site 0x00EBFB36); Ego_r.pdb has ?Init@CSecretWhisperingMan@CV_ParanoidWhispersScript@NScript@@EAEXXZ.
//   Entity slot order (landed Init / OnPredicateFail rows, Ego_r.pdb CEntityScriptBase): 0 dtor, 1 Main, 2 Init,
//   3 GetParentScript, 4 OnPersist, 5 OnPredicateFail, 6 OnInterrupted.
// Layout: Ego_r.pdb CSecretWhisperingMan (entity offsets equal retail): ParentClass +0x14, PMasterData +0x18, members from +0x1c;
//   the parent level script's members sit at Ego_r offset - 0x14 (retail CScriptBase is 0x40).
// Callees: CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0 ("HUD_ORB_GREEN_SMALL" 0x012C7F50); interface
//   slots 477 SetThingPersistent, 350 MiniMapAddMarker; CScriptThing::GetPos (slot 6, 0x004AA980).
class CCharString {
public:
    void* PData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
class C3DVector {                                       // 12 bytes (x, y, z)
public:
    float X, Y, Z;
};
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0 (slot 0), implicit copy
class CCPPointerInfo {
public:
    unsigned long RefCount;
};
template <class T> class CCountedPointer {
public:
    T*              Data;
    CCPPointerInfo* Info;
    CCountedPointer(const CCountedPointer& other) : Data(other.Data), Info(other.Info) { if (Info) ++Info->RefCount; }
};
class CScriptThing : public CBaseClass {                // 12 bytes, vtable 0x01238C8C
public:
    virtual ~CScriptThing();                            // slot 0
    virtual void t01(); virtual void t02(); virtual void t03(); virtual void t04(); virtual void t05();
    virtual const C3DVector& GetPos() const;           // slot 6 (+0x18) 0x004AA980
    CCountedPointer<CScriptThing> PImp;                 // +0x04 Data, +0x08 Info
};
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
class CScriptBase;
class CPersistContext;
class CEntityScriptBase {                               // Ego_r.pdb: vptr, Game +0x04, Me +0x08 (20 bytes)
public:
    virtual ~CEntityScriptBase();                       // slot 0
    virtual void Main() = 0;                            // slot 1
    virtual void Init();                                // slot 2
    virtual CScriptBase* GetParentScript() const = 0;   // slot 3
    virtual void OnPersist(CPersistContext& context);   // slot 4
    virtual void OnPredicateFail();                     // slot 5
    virtual void OnInterrupted();                       // slot 6
    bool IsActiveThreadTerminating() const;             // 0x00F35B30
    CGameScriptInterface* Game;                         // +0x04
    CScriptThing          Me;                           // +0x08
};

namespace NScript {
class CQ_SunnyvaleMasterData;
class CV_ParanoidWhispersScript {
public:
    class CSecretWhisperingMan;
};
class CV_ParanoidWhispersScript::CSecretWhisperingMan : public CEntityScriptBase {
public:
    CV_ParanoidWhispersScript* ParentClass;   // +0x14
    CQ_SunnyvaleMasterData* PMasterData;   // +0x18
    C3DVector StartPos;                                 // +0x1c
private:
    virtual void Init();
};

void CV_ParanoidWhispersScript::CSecretWhisperingMan::Init()
{
    Game->SetThingPersistent(Me, true);
    Game->MiniMapAddMarker(Me, "HUD_ORB_GREEN_SMALL");
    StartPos = Me.GetPos();
}
}