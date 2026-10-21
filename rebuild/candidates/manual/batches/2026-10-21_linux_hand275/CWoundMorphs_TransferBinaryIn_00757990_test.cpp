// Behaviour test for retail 0x00757990: candidate source embedded verbatim, then a working memory stream, a vector
// resize, the map sort, and a main that reads a two-entry block through the fast path and the overflow path.
#include <stdio.h>
#include <stdlib.h>
#ifndef _MSC_VER
#define __fastcall
#define __cdecl
#define __forceinline inline
#define __int64 long long
#define memcpy Test_memcpy
#endif
// CWoundMorphs::TransferBinaryIn(CMemoryDataInputStream* stream) -- reads the entry count, resizes the morph map's vector, reads all the (location, entry) pairs as one block, re-sorts the map if it was dirty, then skips a trailing 4-byte field
// Leaf: TransferBinaryIn
// Retail: Fable.exe (TLC, VC7.1) @ 0x00757990, 402 bytes (+ int3 padding), `ret 4`.
// Callees: CDataInputStream::ReadWithSrcChunkOverflow 0x00993CA0, vector<pair<EBodyLocation, CEntry>>::resize(n,
//   const T&) 0x007595F0 (manifest label vector<CVectorMap<...>>::resize, propagated), the map's sort 0x007597C0
//   (`__fastcall` first/last, comparator by value; manifest "CSoundPairHeap_MakeHeap").
// Layout: Ego_r.pdb CWoundMorphs { CVectorMap<EBodyLocation, CEntry> Morphs } -- the PDB vector is 16 bytes (Compare
//   +0x10, Dirty +0x11), retail STLport's is 12 (Compare +0xC, Dirty +0xD), as the landed CSkeletalMorphs::
//   TransferBinaryIn 0x0081AED0; pair<EBodyLocation, CEntry> is 0x2C bytes (CEntry: 9 longs + float, as the landed
//   CTCWound::AddWound 0x00757B30).
// Shape: the trailing 4-byte field is read back into `count` (retail reads both into the dead stream-arg slot, frame
//   0x30); CEntry's user ctor zeroes the longs and sets the float to 0.0f (the float is the one `mov [],0` immediate,
//   the rest share the ebx zero); the inline Read tests `size <= m_remaining` (`cmp eax,[ebp+0x14]; jg`).
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
        if (size <= m_remaining) {
            memcpy(buffer, m_pData, size);
            m_pData += size;
            m_remaining -= size;
            m_position += size;
        }
        else
            ReadWithSrcChunkOverflow((unsigned char*)buffer, size);
    }
};

enum EBodyLocation { BODY_LOCATION_NONE };

struct CWoundMorphs {
    struct CEntry {
        EBodyLocation Location;
        long          ReplacingTextureIndex;
        long          ReplacingBumpMapIndex;
        long          WoundTextureIndex;
        long          WoundBumpMapIndex;
        long          ScarTextureIndex;
        long          ScarBumpMapIndex;
        long          FadeToScarDuration;
        long          TotalDuration;
        float         PermanentScarStrength;
        CEntry() : Location(BODY_LOCATION_NONE), ReplacingTextureIndex(0), ReplacingBumpMapIndex(0), WoundTextureIndex(0),
            WoundBumpMapIndex(0), ScarTextureIndex(0), ScarBumpMapIndex(0), FadeToScarDuration(0), TotalDuration(0),
            PermanentScarStrength(0.0f) { }
    };
    struct CEntryPair {                                   // pair<EBodyLocation, CEntry>, 0x2C bytes
        EBodyLocation first;
        CEntry        second;
        CEntryPair() : first(EBodyLocation()), second(CEntry()) { }
    };
    struct CCompare { };                                  // CKeyPairCompareLess<EBodyLocation, CEntry>
    struct CEntryVector {
        CEntryPair* _M_start;
        CEntryPair* _M_finish;
        CEntryPair* _M_end_of_storage;
        unsigned long size() const { return _M_finish - _M_start; }
        CEntryPair* GetData() { return size() != 0 ? _M_start : (CEntryPair*)this; }
        void resize(unsigned long count, const CEntryPair& value);   // 0x007595F0
    };
    CEntryVector Morphs;                                  // +0x00
    CCompare     Compare;                                 // +0x0C
    bool         Dirty;                                   // +0x0D
    void TransferBinaryIn(CMemoryDataInputStream* stream);
};
void __fastcall SortEntries(CWoundMorphs::CEntryPair* first, CWoundMorphs::CEntryPair* last, CWoundMorphs::CCompare compare);   // 0x007597C0

void CWoundMorphs::TransferBinaryIn(CMemoryDataInputStream* stream)
{
    unsigned long count;
    stream->Read(&count, sizeof(count));
    Morphs.resize(count, CEntryPair());
    CEntryPair* data = Morphs.GetData();
    long bytes = Morphs.size() * sizeof(CEntryPair);
    if (bytes > 0)
        stream->Read(data, bytes);
    if (Dirty) {
        SortEntries(Morphs._M_start, Morphs._M_finish, Compare);
        Dirty = false;
    }
    stream->Read(&count, sizeof(count));
}

#ifndef _MSC_VER
extern "C" void* Test_memcpy(void* dest, const void* src, unsigned int bytes)
{
    unsigned char* d = (unsigned char*)dest; const unsigned char* s = (const unsigned char*)src;
    for (unsigned int i = 0; i < bytes; ++i) d[i] = s[i];
    return dest;
}
#endif

static int g_overflowCalls = 0;
static int g_sortCalls = 0;
static int g_resizeCalls = 0;
static unsigned char g_buffer[1024];
static unsigned long g_bufferSize = 0;

void CMemoryDataInputStream::ReadWithSrcChunkOverflow(unsigned char* buffer, long size)
{
    ++g_overflowCalls;
    for (long i = 0; i < size; ++i) buffer[i] = m_pData[i];
    m_pData += size;
    m_remaining = 0x10000;            // the next chunk is large
    m_position += size;
}

void CWoundMorphs::CEntryVector::resize(unsigned long count, const CEntryPair& value)
{
    ++g_resizeCalls;
    CEntryPair* storage = (CEntryPair*)malloc((count ? count : 1) * sizeof(CEntryPair));
    for (unsigned long i = 0; i < count; ++i) storage[i] = value;
    free(_M_start);
    _M_start = storage; _M_finish = storage + count; _M_end_of_storage = storage + count;
}

void __fastcall SortEntries(CWoundMorphs::CEntryPair* first, CWoundMorphs::CEntryPair* last, CWoundMorphs::CCompare compare)
{
    ++g_sortCalls;
    for (CWoundMorphs::CEntryPair* i = first; i != last; ++i)
        for (CWoundMorphs::CEntryPair* j = i + 1; j != last; ++j)
            if ((long)j->first < (long)i->first) { CWoundMorphs::CEntryPair t = *i; *i = *j; *j = t; }
}

static void Put(const void* data, unsigned long size)
{
    const unsigned char* p = (const unsigned char*)data;
    for (unsigned long i = 0; i < size; ++i) g_buffer[g_bufferSize++] = p[i];
}

static void BuildBlock()
{
    g_bufferSize = 0;
    unsigned long count = 2;                  // host-width, as the source reads sizeof(count)
    Put(&count, sizeof(count));
    CWoundMorphs::CEntryPair pairs[2];
    pairs[0].first = (EBodyLocation)7; pairs[0].second.Location = (EBodyLocation)7; pairs[0].second.TotalDuration = 70;
    pairs[0].second.PermanentScarStrength = 0.5f;
    pairs[1].first = (EBodyLocation)3; pairs[1].second.Location = (EBodyLocation)3; pairs[1].second.TotalDuration = 30;
    pairs[1].second.PermanentScarStrength = 0.25f;
    Put(pairs, sizeof(pairs));
    unsigned long trailer = 0xABCD;
    Put(&trailer, sizeof(trailer));
}

static int Run(long firstChunk, bool dirty, int expectOverflow, int expectSort)
{
    BuildBlock();
    CMemoryDataInputStream stream;
    stream.m_pVtable = 0; stream.m_position = 0; stream.m_08 = 0; stream.m_pData = g_buffer; stream.m_10 = 0;
    stream.m_remaining = firstChunk;
    CWoundMorphs morphs;
    morphs.Morphs._M_start = morphs.Morphs._M_finish = morphs.Morphs._M_end_of_storage = 0;
    morphs.Dirty = dirty;
    g_overflowCalls = g_sortCalls = g_resizeCalls = 0;
    morphs.TransferBinaryIn(&stream);
    int ok = 1;
    if (morphs.Morphs.size() != 2) ok = 0;
    if (g_resizeCalls != 1 || g_overflowCalls != expectOverflow || g_sortCalls != expectSort) ok = 0;
    if (morphs.Dirty) ok = 0;
    if (stream.m_position != g_bufferSize) ok = 0;
    if (stream.m_pData != g_buffer + g_bufferSize) ok = 0;
    if (ok) {
        CWoundMorphs::CEntryPair* p = morphs.Morphs._M_start;
        long firstKey = dirty ? 3 : 7, secondKey = dirty ? 7 : 3;
        if ((long)p[0].first != firstKey || (long)p[1].first != secondKey) ok = 0;
        if (p[0].second.TotalDuration != firstKey * 10 || p[1].second.TotalDuration != secondKey * 10) ok = 0;
        if (p[0].second.ReplacingTextureIndex != 0 || p[0].second.ScarBumpMapIndex != 0) ok = 0;
    }
    free(morphs.Morphs._M_start);
    if (!ok) printf("case chunk=%ld dirty=%d failed (overflow %d sort %d resize %d)\n", firstChunk, (int)dirty,
        g_overflowCalls, g_sortCalls, g_resizeCalls);
    return ok;
}

int main()
{
    int ok = 1;
    CWoundMorphs::CEntryPair fill;
    if (fill.first != BODY_LOCATION_NONE || fill.second.TotalDuration != 0 || fill.second.PermanentScarStrength != 0.0f) ok = 0;
    ok &= Run(0x10000, false, 0, 0);          // everything in one chunk
    ok &= Run(0x10000, true, 0, 1);           // dirty map gets sorted and marked clean
    ok &= Run(2, true, 1, 1);                 // the count straddles a chunk boundary
    ok &= Run((long)sizeof(unsigned long) + 8, false, 1, 0);            // the pair block straddles a chunk boundary
    ok &= Run((long)(sizeof(unsigned long) + 2 * sizeof(CWoundMorphs::CEntryPair) + sizeof(unsigned long)), false, 0, 0);   // exactly one chunk
    ok &= Run((long)(sizeof(unsigned long) + 2 * sizeof(CWoundMorphs::CEntryPair) + sizeof(unsigned long) - 1), false, 1, 0);   // trailer overflows
    printf(ok ? "00757990_TEST PASS\n" : "00757990_TEST FAIL\n");
    return ok ? 0 : 1;
}
