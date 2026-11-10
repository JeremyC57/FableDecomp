// NUISystem::CRedefinerList::ResetKeysToDefaults(bool wasd) -- reset the profile's key bindings (the WASD or the default set), fade the active redefiner list in, light its accept button (observe the accept event, full alpha) and reset the volume of sound groups 0x10, 0x80 and 0x40
// Leaf: ?ResetKeysToDefaults@CRedefinerList@NUISystem@@SIX_N@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x005565C0, 217 bytes (224 with padding), plain `ret` (Ego_r.pdb
//   `?ResetKeysToDefaults@CRedefinerList@NUISystem@@SIX_N@Z`: static __fastcall, the flag in cl).
// Sound manager (agentSND2, 2026-11-10): PSoundManager is the shared engine/CASoundManager.h one; the local CSoundManagerLH model is gone and its slots carry the header names (SetGainFader slot 46).
// Callees: CTBaseSingleton<CUserProfileManager>::Get 0x0040D2A0, CUserProfileManager::ResetAssignedInputsWASD 0x00408820 /
//   ResetAssignedInputs 0x004085F0, CScrollableList::HideList(float) 0x005559D0 (0.1f), the accept button's CObserver
//   slots 2 IsObservingEvent / 3 ObserveEvent (event 0x19) and its component slots 14 GetColour (CRGBColour by hidden
//   pointer into the `push ecx` frame word) / 13 SetColour, the sound manager's SetGainFader (slot 46) three times.
// Callers: one call at 0x00423080.
// Data: CRedefinerList::s_pActiveList 0x013B8AC4, the sound manager 0x013B8394 (as Cancel 0x00556470).
// Layout: CRedefinerList IsActivated +0x17c, Selecting +0x17d, FadeTime +0x180, PAcceptButton +0x18c (landed
//   CRedefinerList ctor 0x00556350 / OnDeactivate 0x00556580); CComponent's CObserver base at +4. Local views.
// Shape: FadeList(time, selecting) (Ego_r.pdb `?FadeList@CRedefinerList@NUISystem@@QAEXM_N@Z`) is inline: HideList, then
//   FadeTime and Selecting stored through the cached list pointer; the accept-button block re-reads the global under
//   its own null test (VC threads the first null test past it); the colour lighting is the CSlider::AdjustValueToProfile
//   0x0054A6F0 form (GetColour into a local, A = 0xff, then the setter).
class CUserProfileManager {
public:
    static CUserProfileManager& __fastcall Get();       // 0x0040D2A0 (CTBaseSingleton<CUserProfileManager>::Get)
    void ResetAssignedInputs();                         // 0x004085F0
    void ResetAssignedInputsWASD();                     // 0x00408820
};

#include "engine/CASoundManager.h"

class CRGBColour {
public:
    unsigned char B, G, R, A;
    CRGBColour() {}
    CRGBColour(const CRGBColour& c) : B(c.B), G(c.G), R(c.R), A(c.A) {}
};

namespace NUISystem {
class CComponentRoot {                                  // the primary base (vptr at +0)
public:
    virtual void c000(); virtual void c001(); virtual void c002(); virtual void c003(); virtual void c004();
    virtual void c005(); virtual void c006(); virtual void c007(); virtual void c008(); virtual void c009();
    virtual void c010(); virtual void c011(); virtual void c012();
    virtual void SetColour(const CRGBColour& colour);   // slot 13 (+0x34)
    virtual CRGBColour GetColour() const;               // slot 14 (+0x38)
};
class CObserver {                                       // the second base (vptr at +4)
public:
    virtual void o00();
    virtual void o01();
    virtual bool IsObservingEvent(long eventType);      // slot 2 (+0x08)
    virtual void ObserveEvent(long eventType);          // slot 3 (+0x0c)
};
class CComponent : public CComponentRoot, public CObserver {
};

class CRedefinerList {
public:
    virtual void v00();
    unsigned char _body04[0x17c - 0x04];
    bool          IsActivated;                          // +0x17c
    bool          Selecting;                            // +0x17d
    float         FadeTime;                             // +0x180
    bool          HaveUndefinedControls;                // +0x184
    CComponent*   PUndefinedWarning;                    // +0x188
    CComponent*   PAcceptButton;                        // +0x18c
    static CRedefinerList* s_pActiveList;               // 0x013B8AC4
    void HideList(float time);                          // 0x005559D0 (CScrollableList)
    void FadeList(float time, bool selecting)
    {
        HideList(time);
        FadeTime = time;
        Selecting = selecting;
    }
    static void __fastcall ResetKeysToDefaults(bool wasd);
};

void __fastcall CRedefinerList::ResetKeysToDefaults(bool wasd)
{
    if (wasd)
        CUserProfileManager::Get().ResetAssignedInputsWASD();
    else
        CUserProfileManager::Get().ResetAssignedInputs();
    if (s_pActiveList != 0)
        s_pActiveList->FadeList(0.1f, true);
    if (s_pActiveList != 0) {
        CComponent* acceptButton = s_pActiveList->PAcceptButton;
        if (acceptButton != 0 && !acceptButton->IsObservingEvent(0x19)) {
            acceptButton->ObserveEvent(0x19);
            CRGBColour colour = acceptButton->GetColour();
            colour.A = 0xff;
            acceptButton->SetColour(colour);
        }
    }
    if (PSoundManager != 0) {
        PSoundManager->SetGainFader(0x10, 0.0f, 0.0f);
        PSoundManager->SetGainFader(0x80, 0.0f, 0.0f);
        PSoundManager->SetGainFader(0x40, 0.0f, 0.0f);
    }
}
}