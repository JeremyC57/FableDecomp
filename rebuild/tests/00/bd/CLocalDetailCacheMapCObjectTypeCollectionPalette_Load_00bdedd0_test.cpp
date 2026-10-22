#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <new>
#ifndef _MSC_VER
#ifndef _MSC_VER
#include <stdint.h>
#endif
#define __fastcall
#define __cdecl
#define __forceinline inline
#define __declspec(x)
#define __int64 int64_t
#define long int   /* the retail stream format and layout are 32-bit */
#endif
// CLocalDetailCacheMap::CObjectTypeCollectionPalette::Load(CDataInputStream& stream) -- rebuild the palette from a stream: a flag byte and an entry count, then for each entry load a local-detail object collection type and register it with the engine's local detail generator, keeping the registered pointer
// Leaf: Load
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BDEDD0, length 354 bytes (padded to 368), `ret 4`.
// Ego_r.pdb: `?Load@CObjectTypeCollectionPalette@CLocalDetailCacheMap@@QAEXAAVCDataInputStream@@@Z`.
// Callees: CDataInputStream::ReadWithSrcChunkOverflow @ 0x00993CA0 (the inlined Read of the landed 0x006FF810), memmove @ 0x00BFEAE6
//   (the inlined STLport clear()), CBaseClassNonCopyable ctor @ 0x0099A2F0 / dtor @ 0x0099A300, CLocalDetailObjectCollectionType::
//   Load(CDataInputStream&, bool) @ 0x00BE27B0, CEngineLocalDetailGenerator::RegisterObjectCollectionType @ 0x00B59C70 on the
//   generator @ 0x01436E88, vector::_M_insert_overflow @ 0x00BDFC00 (push_back growth).
// Vtables: CLocalDetailObjectCollectionType 0x012A7D94 (its ??_G 0x00BDF740 runs only the base dtor: no user destructor).
// Layout: PDB CObjectTypeCollectionPalette (16) with the 12-byte retail vector: Entries +0 (CArray<const CLocalDetailObjectCollectionType*>).
//   The frame temporary CLocalDetailObjectCollectionType is 0x44 bytes in retail (frame 0x58); its ctor zeroes the three mesh pointers.
// Shape: both header values come from by-value read helpers (homed in the dead stream slot); the per-entry type is a frame
//   temporary built and destroyed each pass; the push_back value is a named pointer local.
#pragma intrinsic(memcpy)


struct CDataInputStream {
    void*                m_pVtable;        // +0x00
    unsigned long        m_position;       // +0x04
    long                 m_08;
    const unsigned char* m_pData;          // +0x0c current chunk read pointer
    long                 m_10;
    long                 m_remaining;      // +0x14 bytes left in the current chunk
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
    __forceinline unsigned char ReadUChar() { unsigned char value; Read(&value, 1); return value; }
    __forceinline long ReadLong() { long value; Read(&value, 4); return value; }
};

struct __true_type { __true_type() { } };
template <class T> inline void _Construct(T* p, const T& value) { new (p) T(value); }

template <class T> inline T* __copy_trivial(const T* first, const T* last, T* result)
{
    if (last == first)
        return result;
    unsigned int n = (const char*)last - (const char*)first;
    return (T*)((char*)memmove(result, first, n) + n);
}

template <class T> struct vector {                 // STLport vector, 12 bytes
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    T* begin() { return _M_start; }
    T* end() { return _M_finish; }
    T* erase(T* first, T* last)
    {
        T* i = __copy_trivial(last, _M_finish, first);
        _M_finish = i;
        return first;
    }
    void clear() { erase(begin(), end()); }
    void _M_insert_overflow(T* position, const T& x, const __true_type&, unsigned long fillLength, bool atEnd);   // 0x00BDFC00
    void push_back(const T& x)
    {
        if (_M_finish != _M_end_of_storage) {
            _Construct(_M_finish, x);
            ++_M_finish;
        } else
            _M_insert_overflow(_M_finish, x, __true_type(), 1UL, true);
    }
};

// Same-TU body of the vector<const CLocalDetailObjectCollectionType*>::_M_insert_overflow instantiation 0x00BDFC00 (retail
// instantiated it in this TU): VC7.1 sees that the __true_type tag is never read, so the empty tag temporary no longer pins
// the dead stream-argument slot and the entry counter can live there.
template <class T> __declspec(noinline) void vector<T>::_M_insert_overflow(T* position, const T& x, const __true_type&, unsigned long fillLength, bool atEnd)
{
    unsigned long oldSize = _M_finish - _M_start;
    unsigned long len = oldSize + (oldSize < fillLength ? fillLength : oldSize);
    T* newStart = len ? (T*)malloc(len * sizeof(T)) : 0;
    T* newFinish = __copy_trivial(_M_start, position, newStart);
    for (unsigned long i = fillLength; i != 0; --i)
        *newFinish++ = x;
    if (!atEnd)
        newFinish = __copy_trivial(position, _M_finish, newFinish);
    if (_M_start)
        free(_M_start);
    _M_start = newStart;
    _M_finish = newFinish;
    _M_end_of_storage = newStart + len;
}

class CBaseClassNonCopyable {
public:
    CBaseClassNonCopyable();                       // 0x0099A2F0
    virtual ~CBaseClassNonCopyable();              // 0x0099A300
};

class CLocalDetailObjectCollectionType : public CBaseClassNonCopyable {   // vtable 0x012A7D94
public:
    void*         Mesh;                            // +0x04 C3DMeshInfo*
    void*         ZSpriteMesh;                     // +0x08
    void*         ShadowMesh;                      // +0x0c
    long          MeshPolyCount;                   // +0x10
    long          MeshBankIndex;                   // +0x14
    long          ZSpriteMeshBankIndex;            // +0x18
    long          ShadowMeshBankIndex;             // +0x1c
    unsigned char PlainDataBlock[0x34];            // +0x20 (0x54-byte object as the PDB)
    CLocalDetailObjectCollectionType() : Mesh(0), ZSpriteMesh(0), ShadowMesh(0) { }
    void Load(CDataInputStream& stream, bool flag);   // 0x00BE27B0
};

class CEngineLocalDetailGenerator {
public:
    const CLocalDetailObjectCollectionType* RegisterObjectCollectionType(const CLocalDetailObjectCollectionType& type);   // 0x00B59C70
};
extern CEngineLocalDetailGenerator* g_pLocalDetailGenerator;   // 0x01436E88

namespace CLocalDetailCacheMap {

class CObjectTypeCollectionPalette {
public:
    vector<const CLocalDetailObjectCollectionType*> Entries;   // +0x00
    void Load(CDataInputStream& stream);
};

void CObjectTypeCollectionPalette::Load(CDataInputStream& stream)
{
    bool flag = stream.ReadUChar() != 0;
    long count = stream.ReadLong();
    Entries.clear();
    for (; count > 0; --count) {
        CLocalDetailObjectCollectionType type;
        type.Load(stream, flag);
        const CLocalDetailObjectCollectionType* registered = g_pLocalDetailGenerator->RegisterObjectCollectionType(type);
        Entries.push_back(registered);
    }
}

}

// ---- behaviour test stubs ----
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { printf("CHECK failed line %d\n", __LINE__); g_fail = 1; } } while (0)
static const unsigned char* g_overflowSrc = 0; static int g_overflows = 0;
void CDataInputStream::ReadWithSrcChunkOverflow(unsigned char* buffer, long size)
{ memcpy(buffer, g_overflowSrc, size); g_overflowSrc += size; m_position += size; ++g_overflows; }
static int g_live = 0;
CBaseClassNonCopyable::CBaseClassNonCopyable() { ++g_live; }
CBaseClassNonCopyable::~CBaseClassNonCopyable() { --g_live; }
static int g_loads = 0; static bool g_flags[8]; static bool g_meshesZero = true;
void CLocalDetailObjectCollectionType::Load(CDataInputStream& stream, bool flag)
{
    if (Mesh || ZSpriteMesh || ShadowMesh) g_meshesZero = false;
    g_flags[g_loads++] = flag; long v; stream.Read(&v, 4); MeshPolyCount = v;
}
static CLocalDetailObjectCollectionType* g_registered[8]; static long g_polys[8]; static int g_regs = 0;
const CLocalDetailObjectCollectionType* CEngineLocalDetailGenerator::RegisterObjectCollectionType(const CLocalDetailObjectCollectionType& type)
{ g_polys[g_regs] = type.MeshPolyCount; return g_registered[g_regs++] = (CLocalDetailObjectCollectionType*)(0x1000 + 0x10 * g_regs); }
static CEngineLocalDetailGenerator g_gen;
CEngineLocalDetailGenerator* g_pLocalDetailGenerator = &g_gen;
static void Put(unsigned char*& p, long v) { memcpy(p, &v, 4); p += 4; }
int main()
{
    unsigned char data[64]; unsigned char* p = data;
    *p++ = 1; Put(p, 3); Put(p, 111); Put(p, 222); Put(p, 333);
    CDataInputStream s; memset(&s, 0, sizeof(s)); s.m_pData = data; s.m_remaining = (long)(p - data);
    CLocalDetailCacheMap::CObjectTypeCollectionPalette pal;
    pal.Entries._M_start = (const CLocalDetailObjectCollectionType**)malloc(2 * sizeof(void*));
    pal.Entries._M_start[0] = 0; pal.Entries._M_finish = pal.Entries._M_start + 1; pal.Entries._M_end_of_storage = pal.Entries._M_start + 2;
    pal.Load(s);
    CHECK(g_loads == 3 && g_flags[0] && g_flags[1] && g_flags[2] && g_meshesZero);
    CHECK(g_polys[0] == 111 && g_polys[1] == 222 && g_polys[2] == 333);
    CHECK(pal.Entries._M_finish - pal.Entries._M_start == 3);
    CHECK(pal.Entries._M_start[0] == g_registered[0] && pal.Entries._M_start[2] == g_registered[2]);
    CHECK(g_live == 0 && s.m_position == 17 && s.m_remaining == 0 && g_overflows == 0);
    // flag 0 and count 0 read through the chunk-overflow path: the palette ends up empty
    unsigned char more[5] = { 0, 0, 0, 0, 0 }; g_overflowSrc = more; s.m_remaining = 0;
    pal.Load(s);
    CHECK(g_overflows == 2 && pal.Entries._M_finish == pal.Entries._M_start && g_loads == 3 && s.m_position == 22);
    // negative count: cleared, nothing loaded
    unsigned char neg[5] = { 0, 0xff, 0xff, 0xff, 0xff }; s.m_pData = neg; s.m_remaining = 5;
    pal.Load(s); CHECK(g_loads == 3 && pal.Entries._M_finish == pal.Entries._M_start);
    if (g_fail) { printf("00bdedd0_TEST FAIL\n"); return 1; }
    printf("00bdedd0_TEST PASS\n");
    return 0;
}