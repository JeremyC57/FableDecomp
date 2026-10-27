// CAnimComponentCombatComboChain::CAnimComponentCombatComboChain() -- default constructor: the counted-pointee root starts at reference count 1, then the class vtable; ComboStage / ComboID are left to SetDefaultValue / the loaders
// Leaf: ??0CAnimComponentCombatComboChain@@QAE@XZ
// Retail: Fable.exe @ 0x005DF6B0, 16 bytes, thiscall, plain `ret` (`mov eax,ecx` first: a constructor).
// Identity (proven): stores vtable 0x012556E8 = CAnimComponentCombatComboChain (rtti-vtables.tsv; RTTI bases
// CAnimComponentBase @0 -> CIVCountedPointeeBase @0); Ego_r.pdb names exactly one constructor,
// ??0CAnimComponentCombatComboChain@@QAE@XZ. No direct caller (the factories inline it; the xref at 0x0061F308 is not a
// code reference): an out-of-line COMDAT.
// Callees: none.
// Shape: the inlined root constructor's IVRefCount = 1 store precedes the vtable store; the bases' own vtable stores are
// dead and dropped (as the landed sibling CAnimComponentStrikeResponseAnim() 0x005DF900).
// Layout: Ego_r.pdb CIVCountedPointeeBase {vptr, IVRefCount +4}, CAnimComponentBase adds nothing, then ComboStage +8,
// ComboID +0xC (16 bytes). No engine header for these classes (local model).
class CIVCountedPointeeBase {
public:
    unsigned long IVRefCount;                               // +0x04
    CIVCountedPointeeBase() : IVRefCount(1) { }
    virtual ~CIVCountedPointeeBase() { }
};

class CAnimComponentBase : public CIVCountedPointeeBase {
public:
    virtual ~CAnimComponentBase() { }
};

class CAnimComponentCombatComboChain : public CAnimComponentBase {   // vtable 0x012556E8
public:
    long ComboStage;                                        // +0x08
    long ComboID;                                           // +0x0C
    CAnimComponentCombatComboChain();
    virtual ~CAnimComponentCombatComboChain();
};

CAnimComponentCombatComboChain::CAnimComponentCombatComboChain()
{
}