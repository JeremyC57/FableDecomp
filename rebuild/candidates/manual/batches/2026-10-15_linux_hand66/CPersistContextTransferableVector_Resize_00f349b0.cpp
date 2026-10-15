// CPersistContext_TransferableVector::Resize(unsigned long) -- resizes the transferred vector of 0x64-byte def records (STLport resize(n, T()) inlined)
// Leaf: Resize
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00F349B0, 53 bytes (+11 int3 padding).
// Callees: 0x00F344C0 record constructor, 0x00F34580 vector::resize(size_type, const T&),
// 0x009FC550 CResource::~CResource.
//
// Referenced from the no-RTTI vtable word at 0x01301C58, right after 0x00F344F0 (the element count,
// `(last-first)/0x64`): slots 0/1 of a CPersistContext_TransferableVector<T> (count, resize; the
// TransferOut/TransferIn hooks follow). The label
// `CPersistContext::Transfer<CAnimationSet>` is propagated and wrong. The body is
// `TransVector->resize(count)` with STLport's one-argument resize inlined: TransVector (+0x4) is
// read first, a default-constructed 0x64-byte record temporary is built in the frame, the
// two-argument resize gets the constructor's returned `this`, and the temporary dies through its
// inline destructor chain: the record's and CDefClassBase's vtable stores are dead, the inline
// CDefPointeeBase destructor stores 0x01230BA0 and the out-of-line CResource destructor runs.
// Hierarchy (RTTI vtables): CIVCountedPointeeBase <- CResource (0x012354A4) <- CDefPointeeBase
// (0x01230BA0) <- CDefClassBase (0x01230BCC) <- record (0x012FB778).
struct CResource {
    virtual ~CResource();                  // 0x009FC550
    unsigned long m_refCount;              // +0x04 (CIVCountedPointeeBase::IVRefCount)
    void*         m_pResourceList;         // +0x08
    CResource*    m_pPrevResource;         // +0x0C
    CResource*    m_pNextResource;         // +0x10
    unsigned long m_resourceSize;          // +0x14
    unsigned long m_lastUsedFrame;         // +0x18
};

struct CDefPointeeBase : CResource {
    virtual ~CDefPointeeBase() {}
};

struct CDefClassBase : CDefPointeeBase {
    virtual ~CDefClassBase() {}
    unsigned char m_defBody[0x0C];         // +0x1C..+0x27 (the constructor clears +0x1C and bits of +0x24)
};

struct CDefRecord64 : CDefClassBase {
    CDefRecord64();                        // 0x00F344C0
    unsigned char m_body[0x64 - sizeof(CDefClassBase)];
};

struct CDefRecordVector {
    CDefRecord64* m_pFirst;
    CDefRecord64* m_pLast;
    CDefRecord64* m_pEnd;
    void resize(unsigned long count, const CDefRecord64& value);   // 0x00F34580
    void resize(unsigned long count) { resize(count, CDefRecord64()); }
};

struct CPersistContext_TransferableVector {
    virtual long GetCount();               // 0x00F344F0
    virtual void Resize(unsigned long count);
    CDefRecordVector* TransVector;         // +0x4
};

void CPersistContext_TransferableVector::Resize(unsigned long count)
{
    TransVector->resize(count);
}
