// CAnimationEntry::GetDelayTime() const -- the entry's CAnimComponentDelayTime component (its own, else the default) and its Delay value
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005DC1F0, length 18 bytes (+ int3 padding).
// Leaf: ?GetDelayTime@CAnimationEntry@@QBEJXZ
// Naming: Ego_r.pdb ?GetDelayTime@CAnimationEntry@@QBEJXZ @ 0x0063BE20; the Ego_r TU has GetTransitionTime 0x0063BE00,
//   GetDelayTime 0x0063BE20, GetFlags 0x0063BE40 in a row, and retail has the three same-shape bodies 0x005DC1D0 /
//   0x005DC1F0 / 0x005DC210 in that order. The callee 0x005E0300 is CAnimationEntry::GetComponent<CAnimComponentDelayTime>: it looks up
//   EAnimComponent key 2 in the entry's component map (+0xC) and falls back to the default-component map; the keys
//   follow the component declaration order (Flags 0, TransitionInTime 1, DelayTime 2: CAnimComponentFlags::Initialise
//   0x0063C040 precedes CAnimComponentTransitionInTime::Initialise 0x0063C0B0 in Ego_r). Ego_r.pdb CAnimComponentDelayTime is
//   CAnimComponentBase (CIVCountedPointeeBase: vptr, IVRefCount) + long Delay at +0x8. Callers: CCreatureActionBase::SetAnimation 0x0069307F, CAnimEntryLite::CAnimEntryLite 0x0073A9C8, CThingCreatureBase::UpdateAnimation, CModeStandBase::SetUpMovementAnim / UpdateIdleAnim (16 calls).
//   FINDINGS chunk 402 (0x005DC1F0; the two siblings are the same family). Was landed as a `__fastcall` stand-in under
//   the propagated catalog name "CCombatActionBase::GetAnimationTransitionInTime" (that PDB symbol is a virtual
//   UBEJXZ of CCombatActionBase, not this shared CAnimationEntry helper).
class CIVCountedPointeeBase {
public:
    virtual ~CIVCountedPointeeBase();
    unsigned long IVRefCount;                       // +0x04
};
class CAnimComponentBase : public CIVCountedPointeeBase { };
class CAnimComponentDelayTime : public CAnimComponentBase {
public:
    long Delay;                                   // +0x08
};

class CAnimationEntry {
public:
    template <class T> void GetComponent(const T** component) const;   // CAnimComponentDelayTime: 0x005E0300
    long GetDelayTime() const;
};

long CAnimationEntry::GetDelayTime() const
{
    const CAnimComponentDelayTime* component;
    GetComponent(&component);
    return component->Delay;
}
