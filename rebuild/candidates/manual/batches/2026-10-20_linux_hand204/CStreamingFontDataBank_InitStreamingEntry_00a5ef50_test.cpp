// Behaviour test for CStreamingFontDataBank::InitStreamingEntry (0x00A5EF50): the "GlyphData" entry is fetched and kept; the old entry is released, reference counts balance.
#include <cstdio>
#include <cstring>
#include <cstdlib>
#ifndef _MSC_VER
#define __fastcall
#endif
struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
};

struct CCPPointerInfo {                                // CCountedPointer's shared block
    long RefCount;                                     // +0
    void (__fastcall* DeleteFunc)(void* data);         // +4
    void* Data;                                        // +8
};

template <class T> struct CCountedPointer {
    T*              Data;
    CCPPointerInfo* Info;
    void Release()
    {
        if (Info) {
            --Info->RefCount;
            if (Info->RefCount == 0) {
                Info->DeleteFunc(Info->Data);
                delete Info;
            }
        }
    }
    ~CCountedPointer() { Release(); }
    CCountedPointer& operator=(const CCountedPointer& other)
    {
        T* data = other.Data;
        CCPPointerInfo* info = other.Info;
        if (Info != info) {
            Release();
            Data = data;
            Info = info;
            if (info)
                ++info->RefCount;
        }
        return *this;
    }
};

class CSymbolMap;
class CBankFileAsyncEntry;

class CStreamingFontDataBank {
public:
    virtual void v00();
    unsigned char                        _bank[0x184];  // CBankFileAsync part, StateBlock +0x164, Fonts +0x178, Filename +0x184
    CCountedPointer<CBankFileAsyncEntry> streamingEntry;   // +0x188
    unsigned long FindIndexBySymbol(const CCharString& symbol) const;               // 0x009CCDC0 (CBankFile)
    CCountedPointer<CBankFileAsyncEntry> GetAsyncEntry(unsigned long index);       // 0x009D58D0 (CBankFileAsync)
    void InitStreamingEntry(CSymbolMap& symbols);
};

void CStreamingFontDataBank::InitStreamingEntry(CSymbolMap& /*symbols*/)
{
    unsigned long index = FindIndexBySymbol("GlyphData");
    streamingEntry = GetAsyncEntry(index);
}
static bool g_armed = false; static void* g_deletedBlocks[4]; static int g_deletes = 0;
void operator delete(void* p) throw() { if (g_armed && g_deletes < 4) g_deletedBlocks[g_deletes++] = p; }   /* spy: records while armed, never frees */
static char g_lookup[32]; static unsigned long g_index; static int g_live = 0; static void* g_destroyed; static int g_destroyCalls = 0;
CCharString::CCharString(const char* text, long) { ++g_live; PStringData = (void*)text; }
CCharString::~CCharString() { --g_live; }
void CStreamingFontDataBank::v00() { }
unsigned long CStreamingFontDataBank::FindIndexBySymbol(const CCharString& s) const { std::strcpy(g_lookup, (const char*)s.PStringData); return 17; }
struct SRawPointer { void* Data; CCPPointerInfo* Info; };
static SRawPointer g_next;
CCountedPointer<CBankFileAsyncEntry> CStreamingFontDataBank::GetAsyncEntry(unsigned long index)
{
    g_index = index;
    if (g_next.Info) ++g_next.Info->RefCount;          /* the returned copy owns one reference */
    return *(CCountedPointer<CBankFileAsyncEntry>*)&g_next;
}
static void __fastcall Destroy(void* data) { ++g_destroyCalls; g_destroyed = data; }
int main()
{
    static CStreamingFontDataBank bank;
    int oldObj = 1, newObj = 2;
    CCPPointerInfo* oldBlock = new CCPPointerInfo; oldBlock->RefCount = 1; oldBlock->DeleteFunc = Destroy; oldBlock->Data = &oldObj;
    CCPPointerInfo* newBlock = new CCPPointerInfo; newBlock->RefCount = 1; newBlock->DeleteFunc = Destroy; newBlock->Data = &newObj;
    bank.streamingEntry.Data = (CBankFileAsyncEntry*)&oldObj; bank.streamingEntry.Info = oldBlock;
    g_next.Data = &newObj; g_next.Info = newBlock;        /* the bank keeps one reference itself */
    g_armed = true;
    bank.InitStreamingEntry(*(CSymbolMap*)&bank);   /* unused */
    g_armed = false;
    bool ok = !std::strcmp(g_lookup, "GlyphData") && g_index == 17 && g_live == 0
        && bank.streamingEntry.Data == (CBankFileAsyncEntry*)&newObj && bank.streamingEntry.Info == newBlock && newBlock->RefCount == 2
        && g_destroyCalls == 1 && g_destroyed == &oldObj && g_deletes == 1 && g_deletedBlocks[0] == oldBlock;
    /* the same entry again: nothing changes */
    g_armed = true;
    bank.InitStreamingEntry(*(CSymbolMap*)&bank);
    g_armed = false;
    ok = ok && newBlock->RefCount == 2 && g_destroyCalls == 1 && g_deletes == 1 && bank.streamingEntry.Info == newBlock;
    if (!ok) { std::printf("00a5ef50_TEST FAIL\n"); return 1; }
    std::printf("00a5ef50_TEST PASS\n");
    return 0;
}
