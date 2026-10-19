#include <cstdio>
#include <cstring>
#ifndef _MSC_VER
#define __fastcall
#define __cdecl
#define __forceinline inline
#define __declspec(x)
#endif
#define free test_free
#define malloc test_malloc
// Behaviour test for 009b0200: the reconstructed source below (copied verbatim from src/) runs against stub callees.
// CParentDefClassBase::InternalSaveBinary(CDataOutputStream&) -- write the CDefClassBase flag bits, re-sort the sub-def info map if it is dirty, then write its count and every (index, sub-def info) entry
// Leaf: ?InternalSaveBinary@CParentDefClassBase
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009B0200, length 224 bytes. Virtual: slot 21 of
// CParentDefClassBase's vtable 0x01230CBC.
// Callees: CDataOutputStream::WriteCBYTE(char) @ 0x00993E50 (x3, the inlined CDefClassBase part -- its
//          out-of-line twin is 0x009ACAB0), WriteUShort @ 0x00993E80 (entry count), WriteULong @ 0x00993EA0
//          (key), WriteLong @ 0x00993EB0 (x2, the two CSubDefInfo fields); the SGI sort of 12-byte
//          elements @ 0x009B84C0 (__fastcall first/last, comparator by value; the landed body is
//          Sort_SElement12_009b84c0, its label "make_heap_partial" is propagated).
// Layout: the retail base is packed (engine/CParentDefClassBase.h: InstantiationName +0x25, SubDefInfoMap
// +0x29); the retail CVectorMap is a 12-byte vector (+0x29 first, +0x2d last, +0x31 end) followed by
// its 1-byte comparator (+0x35) and the "needs sorting" flag (+0x36).
// Shape: base call inlined; CVectorMap::Sort() inline (sort + clear the flag); the count is the
// signed 12-byte element difference; the loop re-reads the vector start for the key and again for the
// value, whose two longs are written through a reference.
struct CDataOutputStream {
    void WriteCBYTE(char value);                   // 0x00993E50
    void WriteUShort(unsigned short value);        // 0x00993E80
    void WriteULong(unsigned long value);          // 0x00993EA0
    void WriteLong(long value);                    // 0x00993EB0
};

struct CSubDefInfo {                               // CDefClassBase::CSubDefInfo
    long DefIndex;
    long Count;
    void Save(CDataOutputStream& stream) const
    {
        stream.WriteLong(DefIndex);
        stream.WriteLong(Count);
    }
};

struct CSubDefInfoPair {                           // pair<unsigned long, CSubDefInfo>, 12 bytes
    unsigned long first;
    CSubDefInfo   second;
};

struct CSubDefKeyCompare { unsigned char _empty; };   // CKeyPairCompareLess<unsigned long, CSubDefInfo>

void __fastcall SortSubDefInfos_009b84c0(CSubDefInfoPair* first, CSubDefInfoPair* last, CSubDefKeyCompare comp);   // 0x009B84C0

#pragma pack(push, 1)
struct CSubDefInfoMap {                            // CVectorMap<unsigned long, CSubDefInfo, ...>
    CSubDefInfoPair*  m_first;                     // +0x00
    CSubDefInfoPair*  m_last;                      // +0x04
    CSubDefInfoPair*  m_end;                       // +0x08
    CSubDefKeyCompare m_comp;                      // +0x0c
    bool              m_needsSort;                 // +0x0d
    void Sort()
    {
        if (m_needsSort) {
            SortSubDefInfos_009b84c0(m_first, m_last, m_comp);
            m_needsSort = false;
        }
    }
    long size() const { return m_last - m_first; }
    CSubDefInfoPair& operator[](unsigned long i) { return m_first[i]; }
};

struct CDefClassBase {
    virtual void v00();
    unsigned char       _body[0x20];               // +0x04 .. +0x24: CResource links, PDefManager, GlobalIndex
    unsigned char       Setup : 1;                 // +0x24 bit 0
    unsigned char       Template : 1;              // +0x24 bit 1
    unsigned char       DefaultValsApplied : 1;    // +0x24 bit 2
    virtual void InternalSaveBinary(CDataOutputStream& stream)
    {
        stream.WriteCBYTE(Setup);
        stream.WriteCBYTE(Template);
        stream.WriteCBYTE(DefaultValsApplied);
    }
};

struct CParentDefClassBase : CDefClassBase {
    long           InstantiationName_TablePos;     // +0x25
    CSubDefInfoMap SubDefInfoMap;                  // +0x29
    virtual void InternalSaveBinary(CDataOutputStream& stream);
};
#pragma pack(pop)

void CParentDefClassBase::InternalSaveBinary(CDataOutputStream& stream)
{
    CDefClassBase::InternalSaveBinary(stream);
    SubDefInfoMap.Sort();
    unsigned long count = SubDefInfoMap.size();
    stream.WriteUShort((unsigned short)count);
    for (unsigned long i = 0; i < count; ++i) {
        stream.WriteULong(SubDefInfoMap[i].first);
        SubDefInfoMap[i].second.Save(stream);
    }
}

// ---- stubs and driver ----
static char g_log[256];
static void Log(const char* s) { std::strcat(g_log, s); std::strcat(g_log, ","); }
static char g_num[32];
void CDataOutputStream::WriteCBYTE(char v) { std::sprintf(g_num, "B%d", (int)v); Log(g_num); }
void CDataOutputStream::WriteUShort(unsigned short v) { std::sprintf(g_num, "S%u", (unsigned)v); Log(g_num); }
void CDataOutputStream::WriteULong(unsigned long v) { std::sprintf(g_num, "U%lu", v); Log(g_num); }
void CDataOutputStream::WriteLong(long v) { std::sprintf(g_num, "L%ld", v); Log(g_num); }
static int g_sorts = 0;
void __fastcall SortSubDefInfos_009b84c0(CSubDefInfoPair* first, CSubDefInfoPair* last, CSubDefKeyCompare)
{   // simple insertion sort on the key
    ++g_sorts;
    for (CSubDefInfoPair* i = first + 1; i < last; ++i)
        for (CSubDefInfoPair* j = i; j > first && j[-1].first > j->first; --j) { CSubDefInfoPair t = *j; *j = j[-1]; j[-1] = t; }
}
void CDefClassBase::v00() {}
int main()
{
    CSubDefInfoPair pairs[2];
    pairs[0].first = 9; pairs[0].second.DefIndex = 90; pairs[0].second.Count = 1;
    pairs[1].first = 3; pairs[1].second.DefIndex = 30; pairs[1].second.Count = 2;
    CParentDefClassBase def;
    def.Setup = 1; def.Template = 0; def.DefaultValsApplied = 1;
    def.SubDefInfoMap.m_first = pairs; def.SubDefInfoMap.m_last = pairs + 2; def.SubDefInfoMap.m_end = pairs + 2;
    def.SubDefInfoMap.m_needsSort = true;
    CDataOutputStream s;
    def.InternalSaveBinary(s);
    bool ok = g_sorts == 1 && !def.SubDefInfoMap.m_needsSort &&
              std::strcmp(g_log, "B1,B0,B1,S2,U3,L30,L2,U9,L90,L1,") == 0;
    g_log[0] = 0;
    def.InternalSaveBinary(s);          // already sorted: no second sort
    ok = ok && g_sorts == 1 && std::strcmp(g_log, "B1,B0,B1,S2,U3,L30,L2,U9,L90,L1,") == 0;
    if (!ok) { std::printf("009b0200_TEST FAIL\n"); return 1; }
    std::printf("009b0200_TEST PASS\n");
    return 0;
}
