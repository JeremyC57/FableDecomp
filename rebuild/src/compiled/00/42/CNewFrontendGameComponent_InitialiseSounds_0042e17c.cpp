// CNewFrontendGameComponent::InitialiseSounds() -- create and load the front-end sample bank, set the front-end sound group's volume and switch the sound manager on
// Leaf: ?InitialiseSounds@CNewFrontendGameComponent@@AAEXXZ
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0042E17C, length 136 bytes, plain `ret` (__fastcall, no arguments).
// Sound manager (agentSND2, 2026-11-10): engine/CASoundManager.h PSoundManager (slot 46 SetGainFader; 0x00991600 under
//   FABLE_RETAIL_CASOUNDMANAGER_ENABLE_NAMES = EnableSound, the Anniversary name agentSND proved; landed as CSoundManagerLH::SetEnabled);
//   CSoundManagerLH is the empty derived class CSoundBankLH's ctor names; was a local padded CSoundManagerLH model.
// Name: FableWin.pdb / Ego_r.pdb `?InitialiseSounds@CNewFrontendGameComponent@@AAEXXZ` (private); single caller 0x0042E9C6 in the
// same TU (CNewFrontendGameComponent::Init); member +0x50 is the PDB's `CCountedPointer<CASoundBank> PSampleBank` (donor +0x50).
// The catalog's "Init_FrontendSounds" is a label.
// Callees: operator new @ 0x00BFEA1A (0x20 bytes), CSoundBankLH::CSoundBankLH(CSoundManagerLH*) @ 0x00A0BAC0 (landed; its argument is
// the owning sound manager, stored at +0xC -- the slot-1 loader reads the manager's +0x40 through it),
// CCountedPointer<CASoundBank>::Reset(CASoundBank*) @ 0x0042FD6F (out of line; the catalog's vector<CCharString> instance label is
// propagated from an identical body), CWideString::CWideString(const wchar_t*) @ 0x0099B6B0 / ~CWideString @ 0x0099B510, the bank's
// slot 1 (+0x04, CSoundBankLH 0x00A0B630: loads the bank file), the sound manager's (0x013B8394, CSoundManagerLH vtable 0x0129D2C4)
// slot 46 SetGainFader (+0xB8 = 0x00A3E9D0: walks the set bits of the mask; Anniversary 360 PDB name, agentSND) and its non-virtual enable setter @ 0x00991600 (bool at +0x24,
// slot 16 called on disable; the catalog's "CWorld::SetMiniMapEnabled" is a propagated label). String: L"data\\sound\\frontend.lug" 0x01230C54.
// Pragmas: #pragma optimize("y", off) + #pragma optimize("s", on) -- the CNewFrontendGameComponent TU (frame pointer, `pop ecx` after new).
// Shape: `PSampleBank.Reset(new CSoundBankLH(g_pSoundSystem))` is VC7.1's null-tested new expression; `esi` becomes &PSampleBank
// (`add esi,0x50`) and is reused for the bank load, whose CWideString temporary is built before the object pointer is loaded;
// the volume call's floats go through `push ecx ; push ecx ; fstp` (fldz / fld1), and the manager pointer is re-read after each call.
#pragma optimize("y", off)
#pragma optimize("s", on)
#include <stddef.h>                                      // wchar_t
class CWideString {
public:
    CWideString(const wchar_t* text);                    // 0x0099B6B0
    ~CWideString();                                      // 0x0099B510
    void* PStringData;
};

class CSoundManagerLH;
class CASoundBank {
public:
    virtual ~CASoundBank();
    virtual void Load(const CWideString& fileName);      // slot 1 (+0x04), CSoundBankLH 0x00A0B630
};
class CSoundBankLH : public CASoundBank {                // vtable 0x0129C9DC, 0x20 bytes
public:
    CSoundBankLH(CSoundManagerLH* manager);              // 0x00A0BAC0
    unsigned char _body[0x20 - 4];
};

#include "engine/CCountedPointer.h"

#define FABLE_RETAIL_CASOUNDMANAGER_ENABLE_NAMES
#include "engine/CASoundManager.h"
class CSoundManagerLH : public CASoundManager { };       // the object at 0x013B8394 (vtable 0x0129D2C4; CSoundBankLH's ctor argument)

class CNewFrontendGameComponent {
public:
    void*                        __vftable;              // +0x00
    unsigned char                _pad04[0x50 - 4];
    CCountedPointer<CASoundBank> PSampleBank;            // +0x50
private:
    void InitialiseSounds();
};

void CNewFrontendGameComponent::InitialiseSounds()
{
    if (PSoundManager) {
        PSampleBank.Reset(new CSoundBankLH(static_cast<CSoundManagerLH*>(PSoundManager)));
        PSampleBank->Load(L"data\\sound\\frontend.lug");
        PSoundManager->SetGainFader(0x8000, 1.0f, 0.0f);
        PSoundManager->EnableSound(true);
    }
}