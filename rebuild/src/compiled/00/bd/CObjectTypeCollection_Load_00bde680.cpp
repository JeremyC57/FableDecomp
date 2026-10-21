// CObjectTypeCollection::Load(CMemoryDataInputStream& stream, const vector<CLocalDetailObjectCollectionType*>& types) -- read the render-update flag, the collection type index (resolved through the type table) and the primitive count, then create, load and link each primitive; false on a missing type or a failed primitive
// Leaf: ?Load@CObjectTypeCollection
// Retail: Fable.exe (TLC, VC7.1) @ 0x00BDE680, length 480 bytes (463 + int3 padding), `ret 8`.
// Callees: CDataInputStream::ReadWithSrcChunkOverflow 0x00993CA0, CObjectTypeCollection::CreatePrimitiveOfType 0x00BDE600;
//   primitive virtuals: slot 8 (+0x20) Load(stream), slot 5 (+0x14) post-load init, slot 0 scalar deleting destructor.
// Layout (retail): Type +0x04, Primitives list head +0x08 (each primitive links through its +0x04), the render-update
//   flag byte +0x14 (Ego_r.pdb places RenderUpdateNeeded at +0x18 after Map/SourceObjects; retail stores it at +0x14).
// Shape: the inlined CMemoryDataInputStream::Read of the landed CReactionMatch_MRPyramid::TransferBinaryIn 0x006FF810 (64-bit
//   position range check, chunk fast path, overflow reader); the flag/index/count reads share one long local homed in the
//   dead stream-argument slot, the per-primitive type id is a second local (homed in the dead `types` slot). The three
//   longs go through an inlined ReadLong() helper: only the helper's local escapes to the overflow reader, so the loop
//   count is not address-taken and its load schedules above the `primitive->Next = Primitives` link store as in retail.
extern "C" void* __cdecl memcpy(void* dest, const void* src, unsigned int bytes);
#pragma intrinsic(memcpy)

struct CMemoryDataInputStream {
    void* m_pVtable;                       // +0
    unsigned long m_position;              // +4
    long m_08;
    const unsigned char* m_pData;          // +0xC  current chunk read pointer
    long m_10;
    long m_remaining;                      // +0x14 bytes left in the current chunk
    void ReadWithSrcChunkOverflow(unsigned char* buffer, long size);   // 0x00993CA0
    static __forceinline bool IsValidPosition(__int64 position) { return position >= 0 && position <= 0x7fffffff; }
    __forceinline void Read(void* buffer, long size)
    {
        __int64 newPosition = (__int64)(m_position + size);
        if (!IsValidPosition(newPosition))
            return;
        if (m_remaining >= size) {
            memcpy(buffer, m_pData, size);
            m_pData += size;
            m_remaining -= size;
            m_position += size;
        }
        else
            ReadWithSrcChunkOverflow((unsigned char*)buffer, size);
    }
    __forceinline long ReadLong() { long v; Read(&v, sizeof(v)); return v; }
};

struct CLocalDetailObjectCollectionType;
struct CLocalDetailObjectCollectionTypeVector {     // STLport vector<CLocalDetailObjectCollectionType*>
    CLocalDetailObjectCollectionType** _M_start;
    CLocalDetailObjectCollectionType** _M_finish;
    CLocalDetailObjectCollectionType** _M_end_of_storage;
    CLocalDetailObjectCollectionType* operator[](long i) const { return _M_start[i]; }
};

struct CLocalDetailPrimitiveBase {
    virtual ~CLocalDetailPrimitiveBase();                           // slot 0
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void OnLoaded();                                        // slot 5 (+0x14)
    virtual void v06();
    virtual void v07();
    virtual bool Load(CMemoryDataInputStream& stream);             // slot 8 (+0x20)
    CLocalDetailPrimitiveBase* Next;                                // +0x04
};

struct CObjectTypeCollection {
    void*                             __vftable;                    // +0x00
    CLocalDetailObjectCollectionType* Type;                         // +0x04
    CLocalDetailPrimitiveBase*        Primitives;                   // +0x08
    CObjectTypeCollection*            Next;                         // +0x0c
    void*                             Map;                          // +0x10
    bool                              RenderUpdateNeeded;           // +0x14
    CLocalDetailPrimitiveBase* CreatePrimitiveOfType(long type);    // 0x00BDE600
    bool Load(CMemoryDataInputStream& stream, const CLocalDetailObjectCollectionTypeVector& types);
};

bool CObjectTypeCollection::Load(CMemoryDataInputStream& stream, const CLocalDetailObjectCollectionTypeVector& types)
{
    long value = stream.ReadLong();
    RenderUpdateNeeded = value != 0;
    value = stream.ReadLong();
    Type = types[value];
    value = stream.ReadLong();
    if (Type == 0)
        return false;
    for (long i = 0; i < value; ++i) {
        long primitiveType;
        stream.Read(&primitiveType, sizeof(primitiveType));
        CLocalDetailPrimitiveBase* primitive = CreatePrimitiveOfType(primitiveType);
        if (primitive == 0)
            return false;
        if (!primitive->Load(stream)) {
            delete primitive;
            return false;
        }
        primitive->OnLoaded();
        primitive->Next = Primitives;
        Primitives = primitive;
    }
    return true;
}