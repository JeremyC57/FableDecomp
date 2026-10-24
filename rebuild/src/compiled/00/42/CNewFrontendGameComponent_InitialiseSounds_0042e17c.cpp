// CNewFrontendGameComponent::InitialiseSounds() -- create and load the front-end sample bank, set the front-end sound group's volume and switch the sound manager on
// Leaf: ?InitialiseSounds@CNewFrontendGameComponent@@AAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0042E17C, length 136 bytes, plain `ret` (__fastcall, no arguments).
// Name: FableWin.pdb / Ego_r.pdb `?InitialiseSounds@CNewFrontendGameComponent@@AAEXXZ` (private); single caller 0x0042E9C6 in the
// same TU (CNewFrontendGameComponent::Init); member +0x50 is the PDB's `CCountedPointer<CASoundBank> PSampleBank` (donor +0x50).
// The catalog's "Init_FrontendSounds" is a label.
// Callees: operator new @ 0x00BFEA1A (0x20 bytes), CSoundBankLH::CSoundBankLH(CSoundManagerLH*) @ 0x00A0BAC0 (landed; its argument is
// the owning sound manager, stored at +0xC -- the slot-1 loader reads the manager's +0x40 through it),
// CCountedPointer<CASoundBank>::Reset(CASoundBank*) @ 0x0042FD6F (out of line; the catalog's vector<CCharString> instance label is
// propagated from an identical body), CWideString::CWideString(const wchar_t*) @ 0x0099B6B0 / ~CWideString @ 0x0099B510, the bank's
// slot 1 (+0x04, CSoundBankLH 0x00A0B630: loads the bank file), the sound manager's (0x013B8394, CSoundManagerLH vtable 0x0129D2C4)
// slot 46 (+0xB8 = 0x00A3E9D0: walks the set bits of the mask) and its non-virtual enable setter @ 0x00991600 (bool at +0x24,
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

template <class T> class CCountedPointer {
public:
    void Reset(T* data);                                 // 0x0042FD6F for CASoundBank
    T* operator->() const { return Data; }
    T*    Data;                                          // +0x00
    void* Info;                                          // +0x04
};

class CSoundManagerLH {                                  // the object at 0x013B8394
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29();
    virtual void v30(); virtual void v31(); virtual void v32(); virtual void v33(); virtual void v34(); virtual void v35();
    virtual void v36(); virtual void v37(); virtual void v38(); virtual void v39(); virtual void v40(); virtual void v41();
    virtual void v42(); virtual void v43(); virtual void v44(); virtual void v45();
    virtual void SetGroupVolume(unsigned long groupMask, float volume, float fadeTime);   // slot 46 (+0xB8) 0x00A3E9D0
    void SetEnabled(bool enabled);                       // 0x00991600
};
extern CSoundManagerLH* g_pSoundSystem;                 // 0x013B8394

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
    if (g_pSoundSystem) {
        PSampleBank.Reset(new CSoundBankLH(g_pSoundSystem));
        PSampleBank->Load(L"data\\sound\\frontend.lug");
        g_pSoundSystem->SetGroupVolume(0x8000, 1.0f, 0.0f);
        g_pSoundSystem->SetEnabled(true);
    }
}