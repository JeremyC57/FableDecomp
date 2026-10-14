// CAnimComponentFollowingOnAnim::Alloc() -- virtual factory: a fresh default-constructed component (ref count 1), or null when the allocation fails
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005DF7F0, vtable slot 4 of CAnimComponentFollowingOnAnim (0x01255788).
// Callees: operator new @ 0x00BFEA1A (returns null on failure: the `test eax,eax` is the `new`
// expression's own null guard around the inlined constructor chain).
// Shape: `return new CAnimComponentFollowingOnAnim;` (0xc bytes). The ref-count store (1 at +4) precedes the
// derived vtable store because it belongs to the root base CIVCountedPointeeBase, whose own vtable
// store is dead and dropped; the intermediate CAnimComponentBase adds only virtuals.
// Member from engine/CAnimComponentFollowingOnAnim.h (PDB name); the -1 store after the vtable is the
// CDefString member default constructor.
struct CDefString { long TablePos; CDefString() : TablePos(-1) { } };
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

struct CAnimComponentFollowingOnAnim : CAnimComponentBase {                // vtable 0x01255788
    CDefString NextAnimName;                    // +0x08 TablePos = -1 (CDefString default constructor, inlined)
    CAnimComponentFollowingOnAnim() { }
    virtual CAnimComponentBase* Alloc() const;
};

CAnimComponentBase* CAnimComponentFollowingOnAnim::Alloc() const
{
    return new CAnimComponentFollowingOnAnim;
}
