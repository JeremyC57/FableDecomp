// CRefCountedEntry_007088b0::CRefCountedEntry_007088b0(long, long, CIVCountedPointer) -- three-member record
// constructor: the intrusive counted pointer copied first (store, AddRef when non-null), then the two longs; the
// by-value pointer argument is released on exit (DecRef, virtual slot 1 when the count hits zero).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007088B0, length 50 bytes.
// Callees: pointee virtual slot 1 (+0x04, delete) from the argument's destructor.
// Layout: +0x00 the counted pointer (copied from the third argument at [esp+10h]), +0x04 and +0x08 the two
// longs from the first and second arguments. The intrusive count lives at pointee+4 (compare the landed
// CIVCountedHolder::operator= @ 0x00710600). The class has no RTTI vtable and the manifest label
// "CCategory::CCategory" is propagated, so the record is named by its address.
struct CIVCountedPointee {
    virtual void v00();
    virtual void Delete();                       // slot 1 (+0x04)
    long IVRefCount;                             // +0x04
};

struct CIVCountedPointer {                       // 4-byte intrusive counted pointer
    CIVCountedPointee* m_pObject;
    CIVCountedPointer(const CIVCountedPointer& other) : m_pObject(other.m_pObject)
    {
        if (m_pObject)
            ++m_pObject->IVRefCount;
    }
    ~CIVCountedPointer()
    {
        if (m_pObject) {
            if (--m_pObject->IVRefCount == 0)
                m_pObject->Delete();
        }
    }
};

struct CRefCountedEntry_007088b0 {
    CIVCountedPointer m_pObject;                 // +0x00
    long              m_first;                   // +0x04
    long              m_second;                  // +0x08
    CRefCountedEntry_007088b0(long first, long second, CIVCountedPointer object);
};

CRefCountedEntry_007088b0::CRefCountedEntry_007088b0(long first, long second, CIVCountedPointer object)
    : m_pObject(object), m_first(first), m_second(second)
{
}