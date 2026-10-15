// Grade: DIFFER(98v96) -- the def loaded into eax then copied to ebx after the filter lea (`mov eax,[ebp-4] ; lea ecx,[ebp-8] ; ... ; mov ebx,eax`) where retail loads ebx directly; the honest CDefPointer-destructor form also re-reads [ebp-4] for the release
// CTCMeleeTrap::ResetTrap() -- stop the continuous animation flag, fetch the owner's appearance def and, when it has an animation named by the global "TRAP_RESET" string (any group), start that animation without looping
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007884F5, length 96 bytes.
// Callees: CDefClassBase::GetSubDef<CAppearanceDef>(CDefPointer<CAppearanceDef const>&) @ 0x0042B0A2 (landed),
// the animation lookup @ 0x005DC2E0 on the def's animation set at +0x34 (name, filter; manifest
// "CAnimationDef_FindAnimationByName"), CTCMeleeTrap::InitialiseAnimation(const CCharString&, bool) @ 0x0078809B, the def's
// virtual slot 1 (Destroy, when the local's release drops the count to zero).
// Globals: the CCharString "TRAP_RESET" @ 0x013BADFC (its landed dynamic initializer @ 0x0121F057); vtable
// CAnimFilterNoGroup @ 0x0122FFDC (RTTI; a 4-byte filter object built in the frame).
// Layout (local model, no engine header): the CTCBase owner at +4 (the thing's PBaseDef at +0x70), PlayingContinuous +0x60.
// Shape: frame pointer + `leave` with free esi/edi/ebx: `optimize("y",off)` + `("s",on)`; the def-pointer local is
// default-constructed null (`and dword ptr [ebp-4],0`), the filter temporary is a polymorphic class whose inline
// constructor stores only its vtable ([ebp-8]) and whose empty destructor leaves nothing; the global string's address is
// kept in edi for both uses; retail caches the def in ebx right after GetSubDef and releases through that register
// (`dec [ebx+4] ; jne`). With a CDefPointer destructor the lane re-reads the escaped slot `[ebp-4]` at the end
// (the wave-24k "CDefPointer local re-read from its slot" gap, 102v96); this best form releases explicitly through a raw
// pointer copy (98v96).
#pragma optimize("y", off)
#pragma optimize("s", on)
#include "fable_string.h"

struct CAnimFilter {
    virtual bool Accepts(const void* entry) const; // slot 0
};

struct CAnimFilterNoGroup : CAnimFilter {
    CAnimFilterNoGroup() { }
    virtual bool Accepts(const void* entry) const;
};

struct CAnimationSet {
    bool FindAnimationByName(const CCharString& name, const CAnimFilter& filter);   // 0x005DC2E0
};

struct CAppearanceDef {                            // counted def: vptr, count at +4
    virtual void v00();
    virtual void Destroy();                        // slot 1
    long          m_count;                         // +0x04
    unsigned char _pad_0x08[0x34 - 0x08];
    CAnimationSet m_animations;                    // +0x34
    __forceinline void Release()
    {
        if (--m_count == 0)
            Destroy();
    }
};

struct CAppearanceDefPointer {                     // CDefPointer<CAppearanceDef const>
    CAppearanceDef* m_pDef;
    CAppearanceDefPointer() : m_pDef(0) { }
    CAppearanceDef* operator->() const { return m_pDef; }
};

struct CDefClassBase {
    bool GetSubDef(CAppearanceDefPointer& out);    // 0x0042B0A2
};

struct CThing {
    unsigned char  _pad_0x00[0x70];
    CDefClassBase* PBaseDef;                       // +0x70
};

extern CCharString g_TrapResetAnimationName;       // 0x013BADFC ("TRAP_RESET")

struct CTCMeleeTrap {
    void*         __vftable;                       // +0x00 (CTCBase)
    CThing*       m_pThing;                        // +0x04 (CTCBase owner)
    unsigned char _pad_0x08[0x60 - 0x08];
    bool          PlayingContinuous;               // +0x60
    void InitialiseAnimation(const CCharString& animationName, bool loop);   // 0x0078809B
    void ResetTrap();
};

void CTCMeleeTrap::ResetTrap()
{
    CAppearanceDefPointer appearance;
    PlayingContinuous = false;
    m_pThing->PBaseDef->GetSubDef(appearance);
    CAppearanceDef* def = appearance.m_pDef;
    if (def->m_animations.FindAnimationByName(g_TrapResetAnimationName, CAnimFilterNoGroup()))
        InitialiseAnimation(g_TrapResetAnimationName, false);
    if (def)
        def->Release();
}
