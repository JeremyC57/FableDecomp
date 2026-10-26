// Behaviour test for 0x004023F0 GetWindowTitle: opens a text bank, looks up TEXT_GUI_WINDOW_TITLE by its CRC; a found
// group entry replaces the default "Fable - The Lost Chapters"; a space is appended; the bank is deleted and the entry
// released on every path.
#include <cstdio>
#include <cstring>
#include <cwchar>

static int g_banks = 0, g_entries = 0, g_opened = 0;
static unsigned long g_wantKey = 0;     // CRC key the bank knows (0 = none)
static const wchar_t* g_entryText = 0;  // text of the group entry (0 = GetPGroupEntry returns null)

class CWideString { public:
    wchar_t s[64];
    CWideString(const wchar_t* t) { std::wcscpy(s, t); }
    CWideString(const CWideString& o) { std::wcscpy(s, o.s); }
    CWideString& operator=(const CWideString& o) { std::wcscpy(s, o.s); return *this; }
};
CWideString operator+(const CWideString& a, const wchar_t* b) { CWideString r(a); std::wcscat(r.s, b); return r; }
class CCharString { public:
    const char* d;
    CCharString(const char* t) : d(t) { }
    const char* GetString() const { return d; }
    unsigned long Length() const { return (unsigned long)std::strlen(d); }
};
static unsigned long crc(unsigned long seed, const void* p, unsigned long n)
{
    const unsigned char* b = (const unsigned char*)p;
    for (unsigned long i = 0; i < n; ++i) { seed ^= b[i]; for (int k = 0; k < 8; ++k) seed = (seed >> 1) ^ (0xEDB88320u & (0u - (seed & 1))); }
    return seed;
}
struct CGroupEntry {
    unsigned long refs;
    CGroupEntry() : refs(1) { ++g_entries; }
    void Release() { if (--refs == 0) { --g_entries; delete this; } }
    CWideString GetText() const { return CWideString(g_entryText); }
};
template <class T> class CIVCountedPointer { public:
    T* Object;
    ~CIVCountedPointer() { if (Object) Object->Release(); }
    T* operator->() const { return Object; }
};
struct CDataBank {
    CDataBank() { ++g_banks; }
    virtual ~CDataBank() { --g_banks; }
    virtual bool OpenReadOnly(const CCharString& name, unsigned long flags) { g_opened = (flags == 0x100 && std::strcmp(name.d, "text.big") == 0); return true; }
    unsigned long LookupValue(unsigned long key) { return (g_wantKey && key == g_wantKey) ? 7 : 0; }
    CIVCountedPointer<CGroupEntry> GetPGroupEntry(unsigned long index, bool) const
    {
        CIVCountedPointer<CGroupEntry> p; p.Object = (index == 7 && g_entryText) ? new CGroupEntry : 0; return p;
    }
};
template <class T> struct auto_ptr {
    T* p; auto_ptr() : p(0) { } ~auto_ptr() { delete p; }
    void reset(T* q) { if (q != p) delete p; p = q; }
    T* operator->() const { return p; }
};
static CCharString GetFontBankName() { return CCharString("text.big"); }

// the function under test (shape of 0x004023F0)
static CWideString GetWindowTitle()
{
    CWideString title(L"Fable - The Lost Chapters");
    auto_ptr<CDataBank> bank;
    CCharString tag("TEXT_GUI_WINDOW_TITLE");
    bank.reset(new CDataBank);
    bank->OpenReadOnly(GetFontBankName(), 0x100);
    unsigned long index = bank->LookupValue(crc(0, tag.GetString(), tag.Length()));
    if (index > 0) {
        CIVCountedPointer<CGroupEntry> entry = bank->GetPGroupEntry(index, true);
        if (entry.Object)
            title = entry->GetText();
    }
    return title + L" ";
}

int main()
{
    int ok = 1;
    const char* tag = "TEXT_GUI_WINDOW_TITLE";
    // no entry in the bank: the default title
    g_wantKey = 0; g_entryText = 0;
    CWideString a = GetWindowTitle();
    if (std::wcscmp(a.s, L"Fable - The Lost Chapters ") != 0 || !g_opened) ok = 0;
    // key known but no group entry: still the default
    g_wantKey = crc(0, tag, (unsigned long)std::strlen(tag)); g_entryText = 0;
    CWideString b = GetWindowTitle();
    if (std::wcscmp(b.s, L"Fable - The Lost Chapters ") != 0) ok = 0;
    // the localised entry
    g_entryText = L"Fable - Die verlorenen Kapitel";
    CWideString c = GetWindowTitle();
    if (std::wcscmp(c.s, L"Fable - Die verlorenen Kapitel ") != 0) ok = 0;
    // a different key does not match
    g_wantKey ^= 1;
    CWideString d = GetWindowTitle();
    if (std::wcscmp(d.s, L"Fable - The Lost Chapters ") != 0) ok = 0;
    if (g_banks != 0 || g_entries != 0) ok = 0;
    std::printf(ok ? "004023f0_TEST PASS\n" : "004023f0_TEST FAIL\n");
    return ok ? 0 : 1;
}