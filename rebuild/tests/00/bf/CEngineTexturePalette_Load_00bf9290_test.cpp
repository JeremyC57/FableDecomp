// Behaviour test for 00bf9290: src/00bf9290.cpp copied verbatim, the declared callees stubbed below so calls and arguments
// are observable. CEngineTexturePalette::Load(CDataInputStream&)
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// CEngineTexturePalette::Load(CDataInputStream& stream) -- read the palette: the retail flag (a retail palette only clears the entry list), else the entry count, the entry names (entry 0 keeps index 0) and for every later entry the graphic-bank index of its name, or the default index when the bank does not know it
// Leaf: Load
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BF9290, length 362 bytes, `ret 4`.
// PDB: ?Load@CEngineTexturePalette@@QAEXAAVCDataInputStream@@@Z.
// Callees: CDataInputStream::ReadWithSrcChunkOverflow @ 0x00993CA0 (the inline Read's slow path), vector<CPaletteEntry>::resize
//   (unsigned) @ 0x00BF97A0 (manifest label propagated), the stream's virtual slot 6 (+0x18, the string read returning
//   CCharString by value), CCharString::operator= @ 0x0099EFB0, ~CCharString @ 0x0099EAE0, the bank's entry lookup by name
//   @ 0x009CCDC0 (const CCharString&, as the landed FindEntry callers).
// Layout: as the landed CEngineTexturePalette::GetTexturePaletteIndex 0x00BF9150: Bank +0, Palette (STLport vector of
//   8-byte {Index, CCharString Name}) +4, DefaultIndex +0x10, RetailVersion +0x14.
// Shape: operator[] as `*(begin_() + n)` through an inline accessor (retry-10 STLport rule). The flag byte and the count are inline by-value stream reads homed in the dead `stream` slot; the count is
//   re-read from that slot for each loop test; the name temporary is a frame word.
extern "C" void* __cdecl memcpy(void* dest, const void* src, unsigned int bytes);
#pragma intrinsic(memcpy)

class CCharString {
public:
    void* PStringData;
    ~CCharString();                                 // 0x0099EAE0
    const CCharString& operator=(const CCharString& other);   // 0x0099EFB0
};

class CDataInputStream {
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual CCharString ReadString();               // slot 6 (+0x18)
    unsigned long m_position;                       // +4
    long StreamSize;
    const unsigned char* m_pData;                   // +0xC
    long SourceChunkStreamPos;
    long m_remaining;                               // +0x14
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
    __forceinline char ReadChar() { char v; Read(&v, sizeof(v)); return v; }
    __forceinline long ReadLong() { long v; Read(&v, sizeof(v)); return v; }
};

class CBankFile {
public:
    unsigned long FindEntry(const CCharString& name);   // 0x009CCDC0
};
class CGraphicDataBank : public CBankFile {};

template <class T> struct CRetailVector {
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    T* begin_() const { return _M_start; }
    T& operator[](unsigned long n) { return *(begin_() + n); }
    void resize(unsigned int n);                    // 0x00BF97A0
};

class CEngineTexturePalette {
public:
    struct CPaletteEntry {
        unsigned long Index;
        CCharString Name;
    };
    CGraphicDataBank* Bank;                         // +0x00
    CRetailVector<CPaletteEntry> Palette;           // +0x04
    unsigned long DefaultIndex;                     // +0x10
    bool RetailVersion;                             // +0x14
    void Load(CDataInputStream& stream);
};

void CEngineTexturePalette::Load(CDataInputStream& stream)
{
    RetailVersion = stream.ReadChar() != 0;
    if (RetailVersion) {
        Palette.resize(0);
        return;
    }
    long count = stream.ReadLong();
    Palette.resize(count);
    Palette[0].Name = stream.ReadString();
    Palette[0].Index = 0;
    for (long i = 1; i < count; ++i) {
        Palette[i].Name = stream.ReadString();
        unsigned long index = Bank->FindEntry(Palette[i].Name);
        if (index > 0)
            Palette[i].Index = index;
        else
            Palette[i].Index = DefaultIndex;
    }
}

// ---- stubs ----
static int g_nextName = 100; static int g_assigns = 0, g_dtors = 0, g_overflow = 0, g_resizeN = -1, g_resizes = 0;
CCharString::~CCharString() { ++g_dtors; }
const CCharString& CCharString::operator=(const CCharString& other) { ++g_assigns; PStringData = other.PStringData; return *this; }
void CDataInputStream::v00() {} void CDataInputStream::v01() {} void CDataInputStream::v02() {}
void CDataInputStream::v03() {} void CDataInputStream::v04() {} void CDataInputStream::v05() {}
CCharString CDataInputStream::ReadString() { CCharString s; s.PStringData = (void*)(long)(g_nextName++); return s; }
static const unsigned char* g_ovSrc = 0;
void CDataInputStream::ReadWithSrcChunkOverflow(unsigned char* buffer, long size) { ++g_overflow; memcpy(buffer, g_ovSrc, size); g_ovSrc += size; m_position += size; }
static CEngineTexturePalette::CPaletteEntry g_entries[8];
template<> void CRetailVector<CEngineTexturePalette::CPaletteEntry>::resize(unsigned int n) { ++g_resizes; g_resizeN = (int)n; _M_start = g_entries; _M_finish = g_entries + n; }
unsigned long CBankFile::FindEntry(const CCharString& name) { long v = (long)name.PStringData; return (v % 2) ? 0 : (unsigned long)v; }
int main()
{
    static CGraphicDataBank bank;
    static unsigned char raw[sizeof(CDataInputStream)]; CDataInputStream* sp = (CDataInputStream*)raw;
    CDataInputStream real; memcpy(raw, &real, sizeof(real));   // copy the vptr
    CEngineTexturePalette pal; pal.Bank = &bank; pal.DefaultIndex = 77; pal.RetailVersion = false;
    // retail palette: only the list is cleared
    static unsigned char data1[] = { 1 };
    sp->m_position = 0; sp->m_pData = data1; sp->m_remaining = 1;
    pal.Load(*sp);
    CHECK(pal.RetailVersion == true); CHECK(g_resizes == 1 && g_resizeN == 0); CHECK(sp->m_position == 1); CHECK(sp->m_remaining == 0);
    // non-retail: count 4 read from the buffer (flag inline, count through the overflow path)
    static unsigned char data2[] = { 0 }; static long cnt = 4; g_ovSrc = (const unsigned char*)&cnt;
    sp->m_position = 0; sp->m_pData = data2; sp->m_remaining = 1; g_nextName = 100; g_assigns = 0; g_dtors = 0;
    pal.Load(*sp);
    CHECK(pal.RetailVersion == false); CHECK(g_overflow == 1); CHECK(g_resizeN == 4);
    CHECK(g_entries[0].Index == 0 && (long)g_entries[0].Name.PStringData == 100);
    CHECK((long)g_entries[1].Name.PStringData == 101 && g_entries[1].Index == 77);   // odd name: bank does not know it
    CHECK((long)g_entries[2].Name.PStringData == 102 && g_entries[2].Index == 102);
    CHECK((long)g_entries[3].Name.PStringData == 103 && g_entries[3].Index == 77);
    CHECK(g_assigns == 4); CHECK(g_dtors >= 4);   // each returned name temporary destroyed (the /Od stub adds its own local)
    // count 1: only entry 0
    static unsigned char data3[] = { 0, 1, 0, 0, 0 };
    sp->m_position = 0; sp->m_pData = data3; sp->m_remaining = 5; g_assigns = 0; g_entries[0].Index = 9;
    pal.Load(*sp);
    CHECK(g_resizeN == 1); CHECK(g_assigns == 1); CHECK(g_entries[0].Index == 0); CHECK(sp->m_position == 5); CHECK(g_overflow == 1);
    // invalid position: the inline reads are skipped (the flag stays the uninitialised local; force through count path is not observable)
    if (fails == 0) printf("00bf9290_TEST PASS\n"); else printf("00bf9290_TEST FAIL\n");
    return fails;
}