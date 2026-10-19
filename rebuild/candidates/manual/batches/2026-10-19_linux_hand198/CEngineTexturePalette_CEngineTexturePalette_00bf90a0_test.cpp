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
// Behaviour test for 00bf90a0: the reconstructed source below (copied verbatim from src/) runs against stub callees.
// CEngineTexturePalette::CEngineTexturePalette(CGraphicDataBank*, bool, unsigned long) -- empty palette over the bank; a non-retail palette starts with one default entry (index 0, empty name)
// Leaf: ??0CEngineTexturePalette
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BF90A0, length 176 bytes (`ret 0Ch`). The manifest label
// "CParticleMorphs::Initialize" is propagated: the only caller is CEngineLandscapeMap's constructor (0x00BDD2F0), which
// builds its TexturePalette member (+0x40) from the engine's graphic bank, false and a creation value.
// Callees: CCharString::CCharString() @ 0x0099E4B0, the CCharString copy ctor @ 0x0099EC30, ~CCharString @ 0x0099EAE0,
//          the STLport vector<CPaletteEntry>::_M_insert_overflow @ 0x00BF94D0.
// Layout: as the landed CEngineTexturePalette::GetTexturePaletteIndex (0x00BF9150): Bank +0, Palette +4 (12-byte
// STLport vector of 8-byte {Index, CCharString Name}), DefaultIndex +0x10, RetailVersion +0x14.
// Shape: the vector's inline constructor nulls its three pointers first, then the scalars are assigned in the body
// (flag, default index, bank); the default entry is a local whose index is zeroed after its name is constructed,
// pushed back through the inline STLport push_back (the copy's index constant-folds to 0).
class CCharString {
public:
    void* PStringData;
    CCharString();                                  // 0x0099E4B0
    CCharString(const CCharString& other);          // 0x0099EC30
    ~CCharString();                                 // 0x0099EAE0
};

class CBankFile { };
class CGraphicDataBank : public CBankFile {};

struct __false_type { __false_type() {} };
inline void* operator new(size_t, void* where) { return where; }

template <class T> struct CRetailVector {
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    CRetailVector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) {}
    void _M_insert_overflow(T* position, const T& x, const __false_type&, unsigned long fillLength, bool atEnd);   // 0x00BF94D0
    void push_back(const T& x)
    {
        if (_M_finish != _M_end_of_storage) {
            new (_M_finish) T(x);
            ++_M_finish;
        } else
            _M_insert_overflow(_M_finish, x, __false_type(), 1, true);
    }
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
    CEngineTexturePalette(CGraphicDataBank* bank, bool retailVersion, unsigned long defaultIndex);
};

CEngineTexturePalette::CEngineTexturePalette(CGraphicDataBank* bank, bool retailVersion, unsigned long defaultIndex)
{
    RetailVersion = retailVersion;
    DefaultIndex = defaultIndex;
    Bank = bank;
    if (!retailVersion) {
        CPaletteEntry entry;
        entry.Index = 0;
        Palette.push_back(entry);
    }
}

// ---- stubs and driver ----
static int g_ctor = 0, g_copy = 0, g_dtor = 0, g_overflow = 0;
static unsigned long g_pushedIndex = 99;
CCharString::CCharString() : PStringData(0) { ++g_ctor; }
CCharString::CCharString(const CCharString& o) : PStringData(o.PStringData) { ++g_copy; }
CCharString::~CCharString() { ++g_dtor; }
template<class T> void CRetailVector<T>::_M_insert_overflow(T* position, const T& x, const __false_type&, unsigned long n, bool atEnd)
{
    ++g_overflow;
    g_pushedIndex = x.Index;
    if (position != _M_finish || n != 1 || !atEnd) g_overflow = 100;
}
int main()
{
    CGraphicDataBank bank;
    CEngineTexturePalette retail(&bank, true, 7);
    bool ok = retail.Bank == &bank && retail.RetailVersion && retail.DefaultIndex == 7 &&
              retail.Palette._M_start == 0 && retail.Palette._M_finish == 0 && retail.Palette._M_end_of_storage == 0 &&
              g_ctor == 0 && g_overflow == 0;
    CEngineTexturePalette dev(&bank, false, 3);
    ok = ok && !dev.RetailVersion && dev.DefaultIndex == 3 && g_ctor == 1 && g_dtor == 1 && g_overflow == 1 && g_pushedIndex == 0;
    // room in the vector: the entry is copy-constructed in place
    CEngineTexturePalette::CPaletteEntry slots[2];
    std::memset(slots, 0xcd, sizeof(slots));
    dev.Palette._M_start = slots; dev.Palette._M_finish = slots; dev.Palette._M_end_of_storage = slots + 2;
    CEngineTexturePalette::CPaletteEntry e; e.Index = 5;
    dev.Palette.push_back(e);
    ok = ok && dev.Palette._M_finish == slots + 1 && slots[0].Index == 5 && g_copy == 1;
    if (!ok) { std::printf("00bf90a0_TEST FAIL\n"); return 1; }
    std::printf("00bf90a0_TEST PASS\n");
    return 0;
}
