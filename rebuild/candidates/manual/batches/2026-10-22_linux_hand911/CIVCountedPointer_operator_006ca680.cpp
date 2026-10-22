// CIVCountedPointer::operator=(const CIVCountedPointer& other) -- intrusive counted-pointer assignment: unless both already hold the same pointee, release the old one (delete it at zero count) and add a reference to the new one; returns *this
// Leaf: ??4CIVCountedPointer
// Retail: Fable.exe (TLC, VC7.1) @ 0x006CA680, 54 bytes, `ret 4`. Unreferenced out-of-line instance (the catalog's
//   "CCountedPointer" label is wrong: CCountedPointer<T> is the 8-byte {Data, Info} pointer in Ego_r.pdb; this one-word,
//   intrusive pointer is CIVCountedPointer<T> = { CIVCountedPointeeBase* Object; }).
// Callees: the pointee's virtual slot 1 (Delete, +4) at zero count.
// Shape: the release is the landed CIVCountedPointer destructor body (0x0084B690: `if (--IVRefCount == 0) Delete();`
//   then `Object = 0`) inside the not-the-same-pointee test; the new pointee is re-read from `other` after it.
// Layout: CIVCountedPointeeBase is modelled as in the landed 0x0084B690 (the engine header flattens the vtable to a
//   void* member, so it cannot carry the virtual Delete): vtable +0, IVRefCount +4.
struct CIVCountedPointeeBase {
    virtual void Destroy(int flags);              // slot 0
    virtual void Delete();                        // slot 1
    unsigned long IVRefCount;                     // +0x04
};

struct CIVCountedPointer {
    CIVCountedPointeeBase* Object;                    // +0x00
    CIVCountedPointer& operator=(const CIVCountedPointer& other);
};

CIVCountedPointer& CIVCountedPointer::operator=(const CIVCountedPointer& other)
{
    if (Object != other.Object) {
        CIVCountedPointeeBase* old = Object;
        if (old) {
            if (--old->IVRefCount == 0)
                old->Delete();
            Object = 0;
        }
        Object = other.Object;
        if (Object)
            ++Object->IVRefCount;
    }
    return *this;
}
