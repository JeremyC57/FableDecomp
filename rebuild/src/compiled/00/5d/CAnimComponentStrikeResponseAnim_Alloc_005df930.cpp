// CAnimComponentStrikeResponseAnim::Alloc() -- virtual factory: a fresh default-constructed component (ref count 1), or null when the allocation fails
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005DF930, vtable slot 4 of CAnimComponentStrikeResponseAnim (0x01255828).
// Callees: operator new @ 0x00BFEA1A (returns null on failure: the `test eax,eax` is the `new`
// expression's own null guard around the inlined constructor chain).
// Shape: `return new CAnimComponentStrikeResponseAnim;` (0xc bytes). The ref-count store (1 at +4) precedes the
// derived vtable store because it belongs to the root base CIVCountedPointeeBase, whose own vtable
// store is dead and dropped; the intermediate CAnimComponentBase adds only virtuals.
// Member from engine/CAnimComponentStrikeResponseAnim.h (PDB name); CCharString::CCharString() @ 0x0099E4B0.
struct CCharString { void* PStringData; CCharString(); };   // 0x0099E4B0
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

struct CAnimComponentStrikeResponseAnim : CAnimComponentBase {                // vtable 0x01255828
    CCharString ResponseAnimName;               // +0x08 default-constructed out of line @ 0x0099E4B0 (the pointer survives the call in esi)
    CAnimComponentStrikeResponseAnim() { }
    virtual CAnimComponentBase* Alloc() const;
};

CAnimComponentBase* CAnimComponentStrikeResponseAnim::Alloc() const
{
    return new CAnimComponentStrikeResponseAnim;
}