// Behaviour test for 00bde680: src/00bde680.cpp copied verbatim, the declared callees stubbed below so calls and arguments
// are observable. CObjectTypeCollection::Load(CMemoryDataInputStream&, const vector<CLocalDetailObjectCollectionType*>&)
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
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

class CMemoryDataInputStream { public:
    void* m_pVtable;                       // +0
    unsigned long m_position;              // +4
    long StreamSize;
    const unsigned char* m_pData;          // +0xC  current chunk read pointer
    long SourceChunkStreamPos;
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

class CLocalDetailObjectCollectionType;
struct CLocalDetailObjectCollectionTypeVector {     // STLport vector<CLocalDetailObjectCollectionType*>
    CLocalDetailObjectCollectionType** _M_start;
    CLocalDetailObjectCollectionType** _M_finish;
    CLocalDetailObjectCollectionType** _M_end_of_storage;
    CLocalDetailObjectCollectionType* operator[](long i) const { return _M_start[i]; }
};

class CLocalDetailPrimitiveBase { public:
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

// ---- stubs ----
static int g_overflow = 0, g_creates = 0, g_dtors = 0, g_loads = 0, g_onloaded = 0, g_failType = -1;
static const unsigned char* g_ovSrc = 0;
void CMemoryDataInputStream::ReadWithSrcChunkOverflow(unsigned char* buffer, long size) { ++g_overflow; memcpy(buffer, g_ovSrc, size); g_ovSrc += size; m_position += size; }
CLocalDetailPrimitiveBase::~CLocalDetailPrimitiveBase() { ++g_dtors; }
void CLocalDetailPrimitiveBase::v01() {} void CLocalDetailPrimitiveBase::v02() {} void CLocalDetailPrimitiveBase::v03() {}
void CLocalDetailPrimitiveBase::v04() {} void CLocalDetailPrimitiveBase::v06() {} void CLocalDetailPrimitiveBase::v07() {}
void CLocalDetailPrimitiveBase::OnLoaded() {}
bool CLocalDetailPrimitiveBase::Load(CMemoryDataInputStream&) { return false; }
struct TPrim : CLocalDetailPrimitiveBase {
    long Kind; long Loaded; long Inited;
    TPrim(long k) : Kind(k), Loaded(0), Inited(0) { Next = (CLocalDetailPrimitiveBase*)0x1234; }
    virtual void OnLoaded() { ++g_onloaded; Inited = 1; }
    virtual bool Load(CMemoryDataInputStream& s) { ++g_loads; Loaded = 1; return Kind != g_failType; }
};
CLocalDetailPrimitiveBase* CObjectTypeCollection::CreatePrimitiveOfType(long type) { ++g_creates; if (type == 0) return 0; return new TPrim(type); }
static void Reset() { g_overflow = g_creates = g_dtors = g_loads = g_onloaded = 0; g_failType = -1; }
static void Setup(CMemoryDataInputStream& s, const long* data, long inlineBytes, const long* ov)
{
    s.m_position = 0; s.m_pData = (const unsigned char*)data; s.m_remaining = inlineBytes; g_ovSrc = (const unsigned char*)ov;
}
int main()
{
    static CMemoryDataInputStream s; static CObjectTypeCollection c;
    static CLocalDetailObjectCollectionType* tab[3] = { 0, (CLocalDetailObjectCollectionType*)0x1000, (CLocalDetailObjectCollectionType*)0x2000 };
    CLocalDetailObjectCollectionTypeVector types; types._M_start = tab; types._M_finish = tab + 3; types._M_end_of_storage = tab + 3;
    // missing type: flag and type set, count read, no primitives created, false
    Reset(); memset(&c, 0, sizeof(c));
    static long d1[] = { 1, 0, 5 };
    Setup(s, d1, 12, 0);
    CHECK(c.Load(s, types) == false);
    CHECK(c.RenderUpdateNeeded == true); CHECK(c.Type == 0); CHECK(g_creates == 0); CHECK(s.m_position == 12); CHECK(s.m_remaining == 0);
    // three primitives, the last two type ids through the overflow reader: linked newest first, each loaded and initialised
    Reset(); memset(&c, 0, sizeof(c)); c.RenderUpdateNeeded = true;
    static long d2[] = { 0, 2, 3, 7 }; static long ov2[] = { 8, 9 };
    Setup(s, d2, 16, ov2);
    CHECK(c.Load(s, types) == true);
    CHECK(c.RenderUpdateNeeded == false); CHECK(c.Type == tab[2]); CHECK(g_creates == 3); CHECK(g_loads == 3); CHECK(g_onloaded == 3);
    CHECK(g_overflow == 2); CHECK(s.m_position == 24);
    TPrim* p = (TPrim*)c.Primitives;
    CHECK(p && p->Kind == 9 && p->Inited == 1);
    p = (TPrim*)p->Next; CHECK(p && p->Kind == 8);
    p = (TPrim*)p->Next; CHECK(p && p->Kind == 7);
    CHECK(p->Next == 0);
    // primitive count 0: true, nothing created
    Reset(); memset(&c, 0, sizeof(c));
    static long d3[] = { 2, 1, 0 };
    Setup(s, d3, 12, 0);
    CHECK(c.Load(s, types) == true); CHECK(c.RenderUpdateNeeded == true); CHECK(c.Type == tab[1]); CHECK(g_creates == 0); CHECK(c.Primitives == 0);
    // create fails on the second primitive: false, the first stays linked
    Reset(); memset(&c, 0, sizeof(c));
    static long d4[] = { 0, 1, 3, 4, 0, 6 };
    Setup(s, d4, 24, 0);
    CHECK(c.Load(s, types) == false); CHECK(g_creates == 2); CHECK(g_loads == 1); CHECK(g_dtors == 0);
    CHECK(c.Primitives && ((TPrim*)c.Primitives)->Kind == 4); CHECK(s.m_position == 20);
    // load fails on the second primitive: it is deleted, not initialised or linked, false
    Reset(); memset(&c, 0, sizeof(c)); g_failType = 5;
    static long d5[] = { 0, 1, 3, 4, 5, 6 };
    Setup(s, d5, 24, 0);
    CHECK(c.Load(s, types) == false); CHECK(g_creates == 2); CHECK(g_loads == 2); CHECK(g_onloaded == 1); CHECK(g_dtors == 1);
    CHECK(c.Primitives && ((TPrim*)c.Primitives)->Kind == 4 && c.Primitives->Next == 0);
    if (fails == 0) printf("00bde680_TEST PASS\n"); else printf("00bde680_TEST FAIL\n");
    return fails;
}