// CAnimComponentFlags::Alloc() -- virtual factory: a fresh default-constructed component (ref count 1), or null when the allocation fails
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005DFE90, vtable slot 4 of CAnimComponentFlags (0x01255850).
// Callees: operator new @ 0x00BFEA1A (returns null on failure: the `test eax,eax` is the `new`
// expression's own null guard around the inlined constructor chain).
// Shape: `return new CAnimComponentFlags;` (0xc bytes). The ref-count store (1 at +4) precedes the
// derived vtable store because it belongs to the root base CIVCountedPointeeBase, whose own vtable
// store is dead and dropped; the intermediate CAnimComponentBase adds only virtuals.
// Member from engine/CAnimComponentFlags.h (PDB name); modelled here with the real vtable layout.
struct CPersistContext; struct CCharString; struct CDataInputStream; struct CDataOutputStream;

struct CIVCountedPointeeBase {                  // root: vptr + intrusive ref count
    virtual void v0();
    long m_refCount;                            // +0x04
    CIVCountedPointeeBase() : m_refCount(1) { }
};

struct CAnimComponentBase : CIVCountedPointeeBase {
    virtual void v1();
    virtual long GetType() const;                                                // slot 2
    virtual bool Initialise(CPersistContext& context, const CCharString& name);  // slot 3
    virtual CAnimComponentBase* Alloc() const;                                   // slot 4
    virtual CAnimComponentBase* Clone() const;                                   // slot 5
    virtual void TransferBinaryIn(CDataInputStream& stream);                     // slot 6
    virtual void TransferBinaryOut(CDataOutputStream& stream) const;             // slot 7
    virtual void SetDefaultValue();                                              // slot 8
};

struct CAnimComponentFlags : CAnimComponentBase {                // vtable 0x01255850
    long Flags;                                 // +0x08 (left uninitialised)
    CAnimComponentFlags() { }
    virtual CAnimComponentBase* Alloc() const;
};

CAnimComponentBase* CAnimComponentFlags::Alloc() const
{
    return new CAnimComponentFlags;
}