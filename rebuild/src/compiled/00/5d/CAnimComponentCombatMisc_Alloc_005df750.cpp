// CAnimComponentCombatMisc::Alloc() const -- virtual factory: a new default CAnimComponentCombatMisc (reference count 1)
// Leaf: ?Alloc@CAnimComponentCombatMisc@@UBEPAVCAnimComponentBase@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x005DF750, 31 bytes (one int3 after the `ret`, then the next function 0x005DF770),
//   thiscall (this unused), `push 0Ah ; call operator new` 0x00BFEA1A, null test, inlined default ctor, `ret`.
// Identity (proven): the only reference is slot 4 of the CAnimComponentCombatMisc vtable 0x01255738 (0x01255748; RTTI
//   .?AVCAnimComponentCombatMisc@@; slot 2 = the landed GetType 0x005DF740 returning 6, slot 3 Initialise 0x005DC7B0, slot 5
//   Clone 0x005DB350, slot 7 TransferBinaryOut 0x005DB370); the sibling factories CAnimComponentCombatRecoil 0x005DF710 /
//   CombatComboChain 0x005DF6D0 / CombatHandedness 0x005DF690 sit in the same slot. Ego_r.pdb name set
//   ?Alloc@CAnimComponentCombatMisc@@UBEPAVCAnimComponentBase@@XZ (a const virtual returning the base pointer); the allocation
//   size 10 is the PDB sizeof(CAnimComponentCombatMisc) = 0xA. The catalog name "CAnimComponentCombatMisc::Alloc" is right.
// Callee: operator new 0x00BFEA1A (MSVCR71 scalar new import thunk).
// Layout: Ego_r.pdb CIVCountedPointeeBase {vptr, ulong IVRefCount +0x4} (8) <- CAnimComponentBase (8) <- CAnimComponentCombatMisc
//   {bool MeleeFlourish +0x8, bool MeleeKnockdown +0x9} = 0xA bytes: the anim-component classes are 1-packed (the PDB also puts
//   CAnimComponentCombatFlourishTargetOffset::TargetOffsetVector at +0x9, size 17). engine/CAnimComponentCombatMisc.h flattens
//   the polymorphic base into `void* __vftable`; engine/CIVCountedPointeeBase.h's retail opt-in (FABLE_RETAIL_DEF_CLASS_BASE)
//   has no default ctor, so it cannot give the inlined `IVRefCount = 1` (header debt); local models, the IV base outside the
//   pack (the anim classes' pack(1) caps its alignment), as the 66 landed `IVRefCount(1)` models.
// Shape: the reference count is set by the inlined base ctor (before the vtable store; the base vtable stores are dead); the
//   two bools stay uninitialised (no init in the ctor; SetDefaultValue 0x005DB340 sets them).
void* __cdecl operator new(unsigned int size);              // 0x00BFEA1A

class CIVCountedPointeeBase {                                // vtable 0x012354D0 (engine/CIVCountedPointeeBase.h retail form)
public:
    CIVCountedPointeeBase() : IVRefCount(1) { }             // FableWin ??0CIVCountedPointeeBase@@QAE@XZ (inline in retail)
    virtual ~CIVCountedPointeeBase() { }                    // slot 0 (scalar deleting dtor 0x004664E0)
    virtual void Delete() const;                            // slot 1: 0x0042D930
    mutable unsigned long IVRefCount;                       // +0x04
};

#pragma pack(push, 1)
class CAnimComponentBase : public CIVCountedPointeeBase {
public:
    virtual long GetType() const;                           // slot 2
    virtual void Initialise();                              // slot 3 (real signature takes a persist context; unused here)
    virtual CAnimComponentBase* Alloc() const;              // slot 4
};

class CAnimComponentCombatMisc : public CAnimComponentBase {   // vtable 0x01255738
public:
    virtual CAnimComponentBase* Alloc() const;
    bool MeleeFlourish;                                     // +0x08
    bool MeleeKnockdown;                                    // +0x09
};
#pragma pack(pop)

CAnimComponentBase* CAnimComponentCombatMisc::Alloc() const
{
    return new CAnimComponentCombatMisc;
}