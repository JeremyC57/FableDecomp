// CIVCountedPointer::CIVCountedPointer(const CIVCountedPointer& other) -- intrusive counted-pointer copy constructor: share other's pointee and add a reference to it
// Leaf: ??0CIVCountedPointer
// Retail: Fable.exe (TLC, VC7.1) @ 0x0083CE60, 20 bytes, `ret 4`. Unreferenced out-of-line instance (catalogued as
//   "CCountedPointer::CCountedPointer"; the one-word intrusive pointer is CIVCountedPointer<T> = { CIVCountedPointeeBase*
//   Object; } in Ego_r.pdb, CCountedPointer<T> is the 8-byte {Data, Info} pair).
// Callees: none.
// Shape: `this` is returned (eax = ecx first), the pointee stored before the null test, then `inc [edx+4]`.
// Layout: CIVCountedPointeeBase is modelled as in the landed 0x0084B690 (the engine header flattens the vtable to a
//   void* member): vtable +0, IVRefCount +4.
struct CIVCountedPointeeBase {
    virtual void Destroy(int flags);              // slot 0
    virtual void Delete();                        // slot 1
    unsigned long IVRefCount;                     // +0x04
};

struct CIVCountedPointer {
    CIVCountedPointeeBase* Object;                // +0x00
    CIVCountedPointer(const CIVCountedPointer& other);
};

CIVCountedPointer::CIVCountedPointer(const CIVCountedPointer& other)
    : Object(other.Object)
{
    if (Object)
        ++Object->IVRefCount;
}