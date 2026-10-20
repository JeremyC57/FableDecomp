#include <cstdio>
#include <new>
#ifndef _MSC_VER
#define __fastcall
#endif
static int g_banks, g_deleted; static bool g_openResult; static unsigned long g_openFlags; static const void* g_openName;
static int TestWcscmp(const unsigned short* a, const unsigned short* b) { while (*a && *a == *b) { ++a; ++b; } return (int)*a - (int)*b; }
static int MemCmp_Unsigned16(const unsigned short* l, const unsigned short* r, unsigned long n) { for (unsigned long i = 0; i < n; ++i) if (l[i] != r[i]) return l[i] < r[i] ? -1 : 1; return 0; }
#define wcscmp TestWcscmp
static void __fastcall DeleteData(void* data);
static const unsigned short s_Empty[] = { 0 };          // L"" 0x0122D70C

struct CWideStringData {                                // STLport basic_string<wchar_t> + NoRefs
    unsigned short* _M_start;
    unsigned short* _M_finish;
    unsigned short* _M_end_of_storage;
    long            NoRefs;
};
class CWideString {
public:
    CWideStringData* PStringData;
};

struct CCPPointerInfo {
    long RefCount;
    void (__fastcall* DeleteFunc)(void* data);
    void* Data;
};
template <class T> struct CCountedPointer {
    T*              Data;
    CCPPointerInfo* Info;
    void Reset(T* data) { Clear(); Data = data; Info = (CCPPointerInfo*)::operator new(sizeof(CCPPointerInfo)); Info->RefCount = 1; Info->DeleteFunc = DeleteData; Info->Data = data; }
    T* operator->() const { return Data; }
    void Clear()
    {
        if (Info) {
            --Info->RefCount;
            if (Info->RefCount == 0) {
                Info->DeleteFunc(Info->Data);
                ::operator delete(Info);
            }
        }
        Info = 0;
        Data = 0;
    }
};

class CBankFileAsync {
public:
    CBankFileAsync() { ++g_banks; }
    virtual ~CBankFileAsync() {}
    virtual bool Open(const CWideString& name, unsigned long flags) = 0;
    unsigned char _body[0x164 - 4];
};
class CWADBank : public CBankFileAsync {               // vtable 0x012442DC
public:
    virtual bool Open(const CWideString& name, unsigned long flags) { g_openName = &name; g_openFlags = flags; return g_openResult; }
};

class CWorldMap {
public:
    unsigned char             _pad00[0xa0];
    CCountedPointer<CWADBank> LevelWADBank;             // +0xa0
    bool                      UseLevelWADBank;          // +0xa8
    void SetAsUsingWAD(const CWideString& wadName);
};

void CWorldMap::SetAsUsingWAD(const CWideString& wadName)
{
    if (wadName.PStringData == 0) {
        if (wcscmp(s_Empty, s_Empty) == 0)
            goto empty;
    }
    else if ((unsigned long)(wadName.PStringData->_M_finish - wadName.PStringData->_M_start) == 0
             && MemCmp_Unsigned16(wadName.PStringData->_M_start, s_Empty, 0) == 0)
        goto empty;
    UseLevelWADBank = true;
    LevelWADBank.Reset(new CWADBank);
    if (!LevelWADBank->Open(wadName, 0x102)) {
        UseLevelWADBank = false;
        LevelWADBank.Clear();
    }
    return;
empty:
    UseLevelWADBank = false;
    LevelWADBank.Clear();
}

static void __fastcall DeleteData(void* data) { ++g_deleted; delete (CWADBank*)data; }
int main()
{
    static CWorldMap map; map.LevelWADBank.Data = 0; map.LevelWADBank.Info = 0; map.UseLevelWADBank = true;
    CWideString none; none.PStringData = 0;
    map.SetAsUsingWAD(none);
    if (map.UseLevelWADBank != false || g_banks != 0) { std::printf("004fdab0_TEST FAIL\n"); return 1; }
    unsigned short text[] = { 'a', '.', 'w', 'a', 'd', 0 };
    CWideStringData data; data._M_start = text; data._M_finish = text + 5; data._M_end_of_storage = text + 5; data.NoRefs = 1;
    CWideString name; name.PStringData = &data;
    g_openResult = true;
    map.SetAsUsingWAD(name);
    if (map.UseLevelWADBank != true || g_banks != 1 || map.LevelWADBank.Data == 0 || g_openName != &name || g_openFlags != 0x102) { std::printf("004fdab0_TEST FAIL\n"); return 1; }
    g_openResult = false;
    map.SetAsUsingWAD(name);   /* replaces the bank (old one released), open fails: dropped */
    if (map.UseLevelWADBank != false || g_banks != 2 || g_deleted != 2 || map.LevelWADBank.Data != 0 || map.LevelWADBank.Info != 0) { std::printf("004fdab0_TEST FAIL\n"); return 1; }
    g_openResult = true; map.SetAsUsingWAD(name);
    CWideStringData empty; empty._M_start = empty._M_finish = empty._M_end_of_storage = text; empty.NoRefs = 1;
    CWideString blank; blank.PStringData = &empty;
    map.SetAsUsingWAD(blank);  /* assigned but empty: dropped */
    if (map.UseLevelWADBank != false || g_deleted != 3 || map.LevelWADBank.Data != 0) { std::printf("004fdab0_TEST FAIL\n"); return 1; }
    std::printf("004fdab0_TEST PASS\n");
    return 0;
}