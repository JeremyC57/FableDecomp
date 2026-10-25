// CAnimationEntry::GetTransitionTime() const -- the entry's CAnimComponentTransitionInTime component (its own, else the default) and its TransitionInTime value
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005DC1D0, length 18 bytes (+ int3 padding).
// Leaf: ?GetTransitionTime@CAnimationEntry@@QBEJXZ
// Naming: Ego_r.pdb ?GetTransitionTime@CAnimationEntry@@QBEJXZ @ 0x0063BE00; the Ego_r TU has GetTransitionTime 0x0063BE00,
//   GetDelayTime 0x0063BE20, GetFlags 0x0063BE40 in a row, and retail has the three same-shape bodies 0x005DC1D0 /
//   0x005DC1F0 / 0x005DC210 in that order. The callee 0x005E0280 is CAnimationEntry::GetComponent<CAnimComponentTransitionInTime>: it looks up
//   EAnimComponent key 1 in the entry's component map (+0xC) and falls back to the default-component map; the keys
//   follow the component declaration order (Flags 0, TransitionInTime 1, DelayTime 2: CAnimComponentFlags::Initialise
//   0x0063C040 precedes CAnimComponentTransitionInTime::Initialise 0x0063C0B0 in Ego_r). Ego_r.pdb CAnimComponentTransitionInTime is
//   CAnimComponentBase (CIVCountedPointeeBase: vptr, IVRefCount) + long TransitionInTime at +0x8. Callers: CAnimEntryLite::CAnimEntryLite 0x0073A9BE, CCreatureAction_FireMissileWeapon::Initialise 0x008D576D.
//   FINDINGS chunk 402 (0x005DC1F0; the two siblings are the same family). Was landed as a `__fastcall` stand-in under
//   the propagated catalog name "CCombatActionBase::GetAnimationTransitionInTime" (that PDB symbol is a virtual
//   UBEJXZ of CCombatActionBase, not this shared CAnimationEntry helper).
class CIVCountedPointeeBase {
public:
    virtual ~CIVCountedPointeeBase();
    unsigned long IVRefCount;                       // +0x04
};
class CAnimComponentBase : public CIVCountedPointeeBase { };
class CAnimComponentTransitionInTime : public CAnimComponentBase {
public:
    long TransitionInTime;                                   // +0x08
};

class CAnimationEntry {
public:
    template <class T> void GetComponent(const T** component) const;   // CAnimComponentTransitionInTime: 0x005E0280
    long GetTransitionTime() const;
};

long CAnimationEntry::GetTransitionTime() const
{
    const CAnimComponentTransitionInTime* component;
    GetComponent(&component);
    return component->TransitionInTime;
}
