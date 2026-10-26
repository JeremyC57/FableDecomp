// CAnimComponentFollowingOnAnimFilter::Alloc() -- virtual factory: a fresh default-constructed component (ref count 1), or null when the allocation fails
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005DF890, vtable slot 4 of CAnimComponentFollowingOnAnimFilter (0x012557D8).
// Callees: operator new @ 0x00BFEA1A (returns null on failure: the `test eax,eax` is the `new`
// expression's own null guard around the inlined constructor chain).
// Shape: `return new CAnimComponentFollowingOnAnimFilter;` (0xc bytes). The ref-count store (1 at +4) precedes the
// derived vtable store because it belongs to the root base CIVCountedPointeeBase, whose own vtable
// store is dead and dropped; the intermediate CAnimComponentBase adds only virtuals.
// Member from engine/CAnimComponentFollowingOnAnimFilter.h (PDB name); the -1 store after the vtable is
// the CDefString member default constructor.
class CDefString { public: long TablePos; CDefString() : TablePos(-1) { } };
class CPersistContext; class CCharString; class CDataInputStream; class CDataOutputStream;

class CIVCountedPointeeBase { public:                  // root: vptr + intrusive ref count
    virtual void v0();
    long m_refCount;                            // +0x04
    CIVCountedPointeeBase() : m_refCount(1) { }
};

class CAnimComponentBase : public CIVCountedPointeeBase { public:
    virtual void v1();
    virtual long GetType() const;                                                // slot 2
    virtual bool Initialise(CPersistContext& context, const CCharString& name);  // slot 3
    virtual CAnimComponentBase* Alloc() const;                                   // slot 4
    virtual CAnimComponentBase* Clone() const;                                   // slot 5
    virtual void TransferBinaryIn(CDataInputStream& stream);                     // slot 6
    virtual void TransferBinaryOut(CDataOutputStream& stream) const;             // slot 7
    virtual void SetDefaultValue();                                              // slot 8
};

class CAnimComponentFollowingOnAnimFilter : public CAnimComponentBase { public:                // vtable 0x012557D8
    CDefString NextFilter;                      // +0x08 TablePos = -1 (CDefString default constructor, inlined)
    CAnimComponentFollowingOnAnimFilter() { }
    virtual CAnimComponentBase* Alloc() const;
};

CAnimComponentBase* CAnimComponentFollowingOnAnimFilter::Alloc() const
{
    return new CAnimComponentFollowingOnAnimFilter;
}