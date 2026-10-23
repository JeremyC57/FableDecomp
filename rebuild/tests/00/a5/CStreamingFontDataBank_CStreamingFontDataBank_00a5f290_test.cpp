// Behaviour test for 00a5f290: body copied from src/00a5f290.cpp (CStreamingFontDataBank::CStreamingFontDataBank), the CBankFile and CCharString callees stubbed.
#include <cstdio>
#include <cstring>
#include <new>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
static int g_bankCtor = 0, g_strCtor = 0, g_strAssign = 0; static const char* g_assigned = 0;
namespace N_009d5f80 { class CBankFile {
public:
    CBankFile() { ++g_bankCtor; }
    virtual ~CBankFile() {}
    unsigned char _body[0x160];
}; } using N_009d5f80::CBankFile;
struct CStateBlock {
    long FontHeight;
    long FontWeight;
    long MinSpacing;
    bool Italics;
};
class CStreamingFontBankStateBlock {
public:
    virtual void Apply() {}
    CStateBlock State;
};
template<class T> struct CArrayAllocProxy {
    T* p;
    CArrayAllocProxy(T* v) : p(v) { }
};
template<class T> struct CArray {
    T*                  _M_start;
    T*                  _M_finish;
    CArrayAllocProxy<T> _M_end_of_storage;
    CArray() : _M_start(0), _M_finish(0), _M_end_of_storage(0) {}
};
class CCharString {
public:
    CCharString() : m_pStringData(0) { ++g_strCtor; }
    CCharString& operator=(const char* text) { ++g_strAssign; g_assigned = text; m_pStringData = (void*)text; return *this; }
    void* m_pStringData;
};
struct CCPPointerInfo;
template<class T> struct CCountedPointer {
    T*              Data;
    CCPPointerInfo* Info;
    CCountedPointer() : Data(0), Info(0) {}
};
struct CStreamingFontDataBankEntry;
struct CBankFileAsyncEntry;
class CStreamingFontDataBank : public CBankFile {
public:
    CStreamingFontDataBank();
    virtual ~CStreamingFontDataBank() {}
    CStreamingFontBankStateBlock         StateBlock;
    CArray<CStreamingFontDataBankEntry*> Fonts;
    CCharString                          Filename;
    CCountedPointer<CBankFileAsyncEntry> streamingEntry;
};
CStreamingFontDataBank::CStreamingFontDataBank()
{
    Filename = "";
}
int main()
{
    unsigned char raw[sizeof(CStreamingFontDataBank)]; memset(raw, 0xAA, sizeof(raw));
    CStreamingFontDataBank* p = new (raw) CStreamingFontDataBank;
    CHECK(g_bankCtor == 1); CHECK(g_strCtor == 1); CHECK(g_strAssign == 1); CHECK(g_assigned != 0 && g_assigned[0] == 0);
    CHECK(p->Fonts._M_start == 0); CHECK(p->Fonts._M_finish == 0); CHECK(p->Fonts._M_end_of_storage.p == 0);
    CHECK(p->streamingEntry.Data == 0); CHECK(p->streamingEntry.Info == 0);
    // the state block is not initialised by this constructor
    CHECK(p->StateBlock.State.FontHeight == (long)0xAAAAAAAA);
    if (fails == 0) printf("00a5f290_TEST PASS\n"); else printf("00a5f290_TEST FAIL\n");
    return fails;
}