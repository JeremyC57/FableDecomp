// Behaviour test for 0x00A633A0 CFontDataBank::CloseFonts.
#include <cstdio>
#include <cstring>
static int g_deleted = 0, g_unloaded = 0, g_canUnload = 0;
class CFontDataBankEntry {
public:
    bool loaded;
    CFontDataBankEntry(bool l) : loaded(l) {}
    virtual ~CFontDataBankEntry() { ++g_deleted; }
    virtual bool IsLoaded() { return loaded; }
    virtual bool CanUnload() { ++g_canUnload; return true; }
    virtual void Unload() { ++g_unloaded; loaded = false; }
};
struct CFontEntryArray {
    CFontDataBankEntry** m_pFirst; CFontDataBankEntry** m_pLast;
    bool empty() { return m_pFirst == m_pLast; }
    CFontDataBankEntry*& back() { return *(m_pLast - 1); }
    void pop_back() { m_pLast = m_pLast - 1; }
};
struct CCharString { const char* s; CCharString& operator=(const char* t) { s = t; return *this; } };
struct CFontDataBank {
    CFontEntryArray Fonts; CCharString Filename;
    void CloseFonts()
    {
        while (!Fonts.empty()) {
            CFontDataBankEntry* entry = Fonts.back();
            if (entry) {
                if (entry->IsLoaded()) { entry->CanUnload(); entry->Unload(); }
                delete entry;
                Fonts.back() = 0;
            }
            Fonts.pop_back();
        }
        Filename = "";
    }
};
int main()
{
    CFontDataBankEntry* arr[3];
    arr[0] = new CFontDataBankEntry(true); arr[1] = 0; arr[2] = new CFontDataBankEntry(false);
    CFontDataBank b; b.Fonts.m_pFirst = arr; b.Fonts.m_pLast = arr + 3; b.Filename.s = "fonts.big";
    b.CloseFonts();
    if (!b.Fonts.empty() || b.Fonts.m_pLast != arr || g_deleted != 2 || g_unloaded != 1 || g_canUnload != 1 ||
        arr[0] != 0 || arr[2] != 0 || std::strlen(b.Filename.s) != 0) { std::printf("00a633a0_TEST FAIL\n"); return 1; }
    std::printf("00a633a0_TEST PASS\n");
    return 0;
}