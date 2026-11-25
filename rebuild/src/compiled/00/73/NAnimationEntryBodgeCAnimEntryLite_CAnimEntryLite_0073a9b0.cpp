// CAnimEntryLite::CAnimEntryLite(const CAnimationEntry& entry) -- cache the entry's bank index, transition time, delay time and flags
// Leaf: ??0CAnimEntryLite@NAnimationEntryBodge@@QAE@ABVCAnimationEntry@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0073A9B0, length 61 bytes (+ int3 padding).
// Callees: CAnimationEntry::GetAnimationTransitionInTime() @ 0x005DC1D0 (component type 1 value),
// CAnimationEntry::GetDelayTime() @ 0x005DC1F0 (component type 2 value; the manifest label is the
// propagated "GetAnimationTransitionInTime"), CAnimationEntry::GetFlagsComponent(T**) @ 0x005E0380 (out pointer,
// inlined GetAnimationFlags: its component's +0x08).
// Layout: PDB CAnimEntryLite (0x10): BankIndex +0x00, DelayTime +0x04, TransitionTime +0x08, AnimFlags +0x0c.
// Shape: body assignments, not an initialiser list -- TransitionTime (+0x08) is computed before DelayTime (+0x04).
// The out-pointer local is homed in the dead argument slot ([esp+0Ch] = the entry reference's slot).
// Name fix (agentNF6 2026-11-10): CAnimationEntry::GetTransitionTime 0x005DC1D0 is ?GetTransitionTime@CAnimationEntry@@QBEJXZ (Ego_r.pdb / FableWin.pdb).
class CAnimComponentFlags { public:
    unsigned char _pad[8];                      // vptr, ref count
    long Flags;                                 // +0x08
};


// Naming (NF442 gen, 2026-11-25): callee-only rebind of this row's calls to their PDB spellings (T1 tracker; the target
//   self rows' Leaf / PDB name sets), through a bodiless declaration (no layout; rulings r2/r3):
//   0x005E0380 ??$GetComponent@VCAnimComponentFlags@@@CAnimationEntry@@QBEXPAPBVCAnimComponentFlags@@@Z (was ?GetFlagsComponent@CAnimationEntry@@QBEXPAPAVCAnimComponentFlags@@@Z)
class CAnimationEntry { public:
    long BankIndex;                             // +0x00
    long AnimName;                              // +0x04 CDefString
    long GroupName;                             // +0x08 CDefString
    void* Components;                           // +0x0c CIVCountedPointer<CAnimComponentMap>
    long GetTransitionTime() const;  // 0x005DC1D0
    long GetAnimationFlags() const
    {
        CAnimComponentFlags* pComponent;
        reinterpret_cast<const CAnimationEntry*>(this)->GetComponent<CAnimComponentFlags>((CAnimComponentFlags const**)(&pComponent));
        return pComponent->Flags;
    }
public:
    long GetDelayTime() const;   // 0x005DC1F0
 public: template <class T1> void GetComponent(T1 const**) const; };

// NF295 (2026-11-23): the shared engine/NAnimationEntryBodge_CAnimEntryLite.h class (same members and offsets) replaces the global
//   local struct, so the row defines the Ego_r + FableWin ??0CAnimEntryLite@NAnimationEntryBodge@@QAE@ABVCAnimationEntry@@@Z that its
//   callers 0x00861E60 / 0x008E9190 / 0x00752640 bind (was ??0CAnimEntryLite@@QAE@ABVCAnimationEntry@@@Z).
#include "engine/NAnimationEntryBodge_CAnimEntryLite.h"

NAnimationEntryBodge::CAnimEntryLite::CAnimEntryLite(const CAnimationEntry& entry)
{
    BankIndex = entry.BankIndex;
    TransitionTime = entry.GetTransitionTime();
    DelayTime = entry.GetDelayTime();
    AnimFlags = entry.GetAnimationFlags();
}