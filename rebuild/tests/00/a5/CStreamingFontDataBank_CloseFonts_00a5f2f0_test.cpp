// Behaviour test for CStreamingFontDataBank::CloseFonts (0x00A5F2F0): loaded fonts are unloaded, every entry deleted, the table emptied and the file name cleared.
#include <cstdio>
#include <cstring>
class CCharString { public:
    void* PStringData;
    CCharString& operator=(const char* text);           // 0x0099EFE0
};

class CStreamingFontDataBankEntry {                     // vtable 0x0129D5B4
public:
    virtual ~CStreamingFontDataBankEntry();             // slot 0 (deleting destructor)
    virtual bool IsLoaded() const;                      // slot 1 (0x00A5F220)
    virtual bool CanUnload() const;                     // slot 2 (0x00A5F230)
    virtual void Unload();                              // slot 3 (0x00A5F250)
};

template <class T> class CArray { public:                      // retail CArray (STLport vector): first / last / end
    T* m_first;
    T* m_last;
    T* m_end;
    static T* CopyTrivial(const T* first, const T* last, T* result)
    {
        if (first == last)
            return result;
        unsigned int n = (const char*)last - (const char*)first;
        return (T*)((char*)memmove(result, first, n) + n);
    }
    unsigned long size() const { return m_last - m_first; }
    T* begin() { return m_first; }
    T* end() { return m_last; }
    T& operator[](unsigned long n) { return *(begin() + n); }
    T* erase(T* first, T* last)
    {
        T* i = CopyTrivial(last, m_last, first);
        m_last = i;
        return first;
    }
    void clear() { erase(begin(), end()); }
};

class CStreamingFontDataBank {
public:
    virtual void v00();
    unsigned char                        _bank[0x160];  // CBankFileAsync part
    unsigned char                        StateBlock[0x14];   // +0x164 CStreamingFontBankStateBlock
    CArray<CStreamingFontDataBankEntry*> Fonts;         // +0x178
    CCharString                          Filename;      // +0x184
    void CloseFonts();
};

void CStreamingFontDataBank::CloseFonts()
{
    for (unsigned long i = 0; i < Fonts.size(); ++i) {
        CStreamingFontDataBankEntry* font = Fonts[i];
        if (font) {
            if (font->IsLoaded()) {
                font->CanUnload();
                font->Unload();
            }
            delete font;
        }
    }
    Fonts.clear();
    Filename = "";
}
static int g_deleted = 0, g_unloaded = 0, g_canUnload = 0; static const char* g_name;
CCharString& CCharString::operator=(const char* text) { g_name = text; PStringData = (void*)text; return *this; }
struct TestFont : CStreamingFontDataBankEntry {
    bool loaded;
    TestFont(bool l) : loaded(l) { }
    ~TestFont() { ++g_deleted; }
};
CStreamingFontDataBankEntry::~CStreamingFontDataBankEntry() { }
bool CStreamingFontDataBankEntry::IsLoaded() const { return static_cast<const TestFont*>(this)->loaded; }
bool CStreamingFontDataBankEntry::CanUnload() const { ++g_canUnload; return true; }
void CStreamingFontDataBankEntry::Unload() { ++g_unloaded; static_cast<TestFont*>(this)->loaded = false; }
void CStreamingFontDataBank::v00() { }
int main()
{
    static CStreamingFontDataBank bank;
    CStreamingFontDataBankEntry* table[4];
    table[0] = new TestFont(true); table[1] = 0; table[2] = new TestFont(false); table[3] = new TestFont(true);
    bank.Fonts.m_first = table; bank.Fonts.m_last = table + 4; bank.Fonts.m_end = table + 4;
    bank.Filename.PStringData = (void*)"fonts.big";
    bank.CloseFonts();
    bool ok = g_deleted == 3 && g_unloaded == 2 && g_canUnload == 2 && bank.Fonts.m_last == table && bank.Fonts.m_first == table
        && g_name != 0 && g_name[0] == 0;
    if (!ok) { std::printf("00a5f2f0_TEST FAIL\n"); return 1; }
    std::printf("00a5f2f0_TEST PASS\n");
    return 0;
}