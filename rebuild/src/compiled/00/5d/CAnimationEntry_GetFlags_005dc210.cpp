// CAnimationEntry::GetFlags() const -- the entry's CAnimComponentFlags component (its own, else the default) and its Flags value
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005DC210, length 18 bytes (+ int3 padding).
// Leaf: ?GetFlags@CAnimationEntry@@QBEJXZ
// Naming: Ego_r.pdb ?GetFlags@CAnimationEntry@@QBEJXZ @ 0x0063BE40; the Ego_r TU has GetTransitionTime 0x0063BE00,
//   GetDelayTime 0x0063BE20, GetFlags 0x0063BE40 in a row, and retail has the three same-shape bodies 0x005DC1D0 /
//   0x005DC1F0 / 0x005DC210 in that order. The callee 0x005E0380 is CAnimationEntry::GetComponent<CAnimComponentFlags>: it looks up
//   EAnimComponent key 0 in the entry's component map (+0xC) and falls back to the default-component map; the keys
//   follow the component declaration order (Flags 0, TransitionInTime 1, DelayTime 2: CAnimComponentFlags::Initialise
//   0x0063C040 precedes CAnimComponentTransitionInTime::Initialise 0x0063C0B0 in Ego_r). Ego_r.pdb CAnimComponentFlags is
//   CAnimComponentBase (CIVCountedPointeeBase: vptr, IVRefCount) + long Flags at +0x8. Callers: CThingCreatureBase::PreloadAnims 0x0066317B and the CCreatureActionBase flag tests OverrideFaceWithDefault 0x00692883 / AllowsCombinationAnims 0x00692893 / CanAnimationTransitionOut 0x006928B3.
//   FINDINGS chunk 402 (0x005DC1F0; the two siblings are the same family). Was landed as a `__fastcall` stand-in under
//   the propagated catalog name "CCombatActionBase::GetAnimationTransitionInTime" (that PDB symbol is a virtual
//   UBEJXZ of CCombatActionBase, not this shared CAnimationEntry helper).
class CIVCountedPointeeBase {
public:
    virtual ~CIVCountedPointeeBase();
    unsigned long IVRefCount;                       // +0x04
};
class CAnimComponentBase : public CIVCountedPointeeBase { };
class CAnimComponentFlags : public CAnimComponentBase {
public:
    long Flags;                                   // +0x08
};

class CAnimationEntry {
public:
    template <class T> void GetComponent(const T** component) const;   // CAnimComponentFlags: 0x005E0380
    long GetFlags() const;
};

long CAnimationEntry::GetFlags() const
{
    const CAnimComponentFlags* component;
    GetComponent(&component);
    return component->Flags;
}
