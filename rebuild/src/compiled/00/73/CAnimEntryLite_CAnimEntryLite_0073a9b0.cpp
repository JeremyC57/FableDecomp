// CAnimEntryLite::CAnimEntryLite(const CAnimationEntry& entry) -- cache the entry's bank index, transition time, delay time and flags
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0073A9B0, length 61 bytes (+ int3 padding).
// Callees: CAnimationEntry::GetAnimationTransitionInTime() @ 0x005DC1D0 (component type 1 value),
// CAnimationEntry::GetAnimationDelayTime() @ 0x005DC1F0 (component type 2 value; the manifest label is the
// propagated "GetAnimationTransitionInTime"), CAnimationEntry::GetFlagsComponent(T**) @ 0x005E0380 (out pointer,
// inlined GetAnimationFlags: its component's +0x08).
// Layout: PDB CAnimEntryLite (0x10): BankIndex +0x00, DelayTime +0x04, TransitionTime +0x08, AnimFlags +0x0c.
// Shape: body assignments, not an initialiser list -- TransitionTime (+0x08) is computed before DelayTime (+0x04).
// The out-pointer local is homed in the dead argument slot ([esp+0Ch] = the entry reference's slot).
struct CAnimComponentFlags {
    unsigned char _pad[8];                      // vptr, ref count
    long Flags;                                 // +0x08
};


struct CAnimationEntry {
    long BankIndex;                             // +0x00
    long AnimName;                              // +0x04 CDefString
    long GroupName;                             // +0x08 CDefString
    void* Components;                           // +0x0c CIVCountedPointer<CAnimComponentMap>
    long GetAnimationTransitionInTime_005dc1d0() const;  // 0x005DC1D0
    long GetAnimationDelayTime() const;         // 0x005DC1F0
    void GetFlagsComponent(CAnimComponentFlags** ppComponent) const;    // 0x005E0380 (out pointer)
    long GetAnimationFlags() const
    {
        CAnimComponentFlags* pComponent;
        GetFlagsComponent(&pComponent);
        return pComponent->Flags;
    }
};

struct CAnimEntryLite {
    long BankIndex;                             // +0x00
    long DelayTime;                             // +0x04
    long TransitionTime;                        // +0x08
    long AnimFlags;                             // +0x0c
    CAnimEntryLite(const CAnimationEntry& entry);
};

CAnimEntryLite::CAnimEntryLite(const CAnimationEntry& entry)
{
    BankIndex = entry.BankIndex;
    TransitionTime = entry.GetAnimationTransitionInTime_005dc1d0();
    DelayTime = entry.GetAnimationDelayTime();
    AnimFlags = entry.GetAnimationFlags();
}