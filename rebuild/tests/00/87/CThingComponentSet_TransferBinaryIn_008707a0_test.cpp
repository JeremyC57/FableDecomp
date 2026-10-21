#include <cstdio>
#include <cstring>
#if !defined(_MSC_VER)
#define __int64 long long
#define __fastcall
#define __cdecl
#define __stdcall
#define __forceinline inline
#define __declspec(x)
#endif
#if !defined(_MSC_VER)
#include <stdint.h>
#undef __int64
#define __int64 int64_t
#define long int  /* the retail stream fields and reads are 4 bytes */
#define memcpy test_memcpy
extern "C" void* test_memcpy(void* d, const void* s, unsigned int n) { return std::memmove(d, s, n); }
#endif
// CThingComponentSet::TransferBinaryIn(CMemoryDataInputStream* stream) -- read the entry count, resize the entry vector to it (new entries default), read the raw 9-byte entries over it and skip the end marker
// Leaf: TransferBinaryIn
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008707A0, length 327 bytes (`ret 4`). The inverse of the landed
//   CThingComponentSet::TransferBinaryOut (0x008708F0: count, raw entries, end marker 0x01AB6CCE).
// Callees: CSmallVector<CThingComponentSet::CEntry, 8>::resize(long, const CEntry&) @ 0x00870370 (as landed 0x00870480;
//   manifest label names the CANavQuadTreeNode instantiation, propagated), CDataInputStream::ReadWithSrcChunkOverflow
//   @ 0x00993CA0 (when the current chunk runs out).
// Layout: Entries.Start +0, Entries.Size (signed char) +4, CEntry 9 bytes packed (Ego_r.pdb, as the landed siblings);
//   CMemoryDataInputStream position +4, chunk read pointer +0xC, bytes left in the chunk +0x14 (as landed 0x007AD7F0).
// Shape: the inlined stream Read (64-bit position range check, inline memcpy or the overflow call) three times; the count
//   is homed in the dead stream-argument slot, the end marker's value is never used (its copy is dropped); the byte count
//   of the entry read is tested `> 0` first; the chunk test is written `size <= m_remaining` (`cmp size,[+14h] ; jg`).
extern "C" void* __cdecl memcpy(void* dest, const void* src, unsigned int bytes);
#pragma intrinsic(memcpy)

#pragma pack(push, 1)
struct CDefString {
    long TablePos;
    CDefString() : TablePos(-1) { }
};

struct CEntry {                         // CThingComponentSet::CEntry, 9 bytes (PDB: packed)
    CDefString Name;                    // +0x00
    unsigned long DriverType;           // +0x04
    bool Override;                      // +0x08
    CEntry() : DriverType(0), Override(false) { }
};
#pragma pack(pop)

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
        if (size <= m_remaining) {
            memcpy(buffer, m_pData, size);
            m_pData += size;
            m_remaining -= size;
            m_position += size;
        }
        else
            ReadWithSrcChunkOverflow((unsigned char*)buffer, size);
    }
    __forceinline long ReadLong()
    {
        long value;
        Read(&value, sizeof(value));
        return value;
    }
};

template<class T, int N> struct CSmallVector {
    T* Start;                           // +0x00
    signed char Size;                   // +0x04
    void resize(long n, const T& fill);     // 0x00870370
    T* GetData() { return Size > 0 ? Start : (T*)this; }
};

struct CThingComponentSet {
    CSmallVector<CEntry, 8> Entries;    // +0x00
    void TransferBinaryIn(CMemoryDataInputStream* stream);
};

void CThingComponentSet::TransferBinaryIn(CMemoryDataInputStream* stream)
{
    long count = stream->ReadLong();
    Entries.resize(count, CEntry());
    CEntry* data = Entries.GetData();
    long bytes = Entries.Size * sizeof(CEntry);
    if (bytes > 0)
        stream->Read(data, bytes);
    stream->ReadLong();
}


static long g_resizeN = -99; static int g_overflows = 0;
static CEntry g_store[8];
template<class T, int N> void CSmallVector<T, N>::resize(long n, const T& fill)
{
    g_resizeN = n;
    if (!(fill.Name.TablePos == -1 && fill.DriverType == 0 && !fill.Override)) return;
    Start = g_store; Size = (signed char)n;
    for (long i = 0; i < n; ++i) g_store[i] = fill;
}
void CMemoryDataInputStream::ReadWithSrcChunkOverflow(unsigned char* buffer, long size)
{
    ++g_overflows;
    memcpy(buffer, m_pData, size); m_pData += size; m_position += size; m_remaining = 0;
}
#define CHECK(c) if (!(c)) { std::printf("008707a0_TEST FAIL line %d\n", __LINE__); return 1; }

int main()
{
    unsigned char buf[64];
    long two = 2, marker = 0x01AB6CCE;
    memcpy(buf, &two, 4);
    for (int i = 0; i < 18; ++i) buf[4 + i] = (unsigned char)(0x10 + i);
    memcpy(buf + 22, &marker, 4);
    CMemoryDataInputStream st;
    st.m_pVtable = 0; st.m_position = 0; st.m_08 = 0; st.m_pData = buf; st.m_10 = 0; st.m_remaining = 26;
    CThingComponentSet set;
    set.Entries.Start = 0; set.Entries.Size = 0;
    set.TransferBinaryIn(&st);
    CHECK(g_resizeN == 2 && set.Entries.Size == 2 && set.Entries.Start == g_store);
    CHECK(((unsigned char*)g_store)[0] == 0x10 && ((unsigned char*)g_store)[17] == 0x21);
    CHECK(st.m_position == 26 && st.m_pData == buf + 26 && st.m_remaining == 0 && g_overflows == 0);
    // short chunk: the entries and the marker go through the overflow path
    st.m_position = 0; st.m_pData = buf; st.m_remaining = 6; g_store[0].DriverType = 0;
    set.TransferBinaryIn(&st);
    CHECK(g_overflows == 2 && st.m_position == 26 && ((unsigned char*)g_store)[17] == buf[21]);
    // empty set: no entry read at all, only count and marker
    long zero = 0; memcpy(buf, &zero, 4); memcpy(buf + 4, &marker, 4);
    st.m_position = 0; st.m_pData = buf; st.m_remaining = 8; g_overflows = 0;
    set.TransferBinaryIn(&st);
    CHECK(g_resizeN == 0 && st.m_position == 8 && st.m_remaining == 0 && g_overflows == 0);
    std::printf("008707a0_TEST PASS\n");
    return 0;
}