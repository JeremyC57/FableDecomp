// Behaviour test for 0054f4b0: a copy of the source's model and logic with its out-of-line callees stubbed.
#include <cstdio>
#include <cstdlib>
#include <cstring>
#ifdef _MSC_VER
typedef unsigned __int64 u64;
#else
typedef unsigned long long u64;
#endif
static int g_fail = 0;
static void Check(bool ok, int line)
{
    if (!ok) {
        g_fail = 1;
        std::printf("check failed at line %d\n", line);
    }
}
#define CHECK(c) Check((c) ? true : false, __LINE__)

static int g_formats = 0, g_getFonts = 0, g_bankDestroyed = 0;
static char g_requestedFont[32];
class CWideString {
public:
    unsigned short Text[8];
    CWideString() { Text[0] = 0; }
    const unsigned short* GetAsCharArray() const { return Text; }
    void Sprintf(const char* format, unsigned short c);   // stub for CWideString::Sprintf
};
void CWideString::Sprintf(const char* format, unsigned short c)
{
    CWideString* out = this;
    ++g_formats;
    CHECK(format[0] == '%' && format[1] == 'c' && format[2] == 0);
    out->Text[0] = c;
    out->Text[1] = 0;
}
class CCharString {
public:
    const char* PData;
    CCharString(const char* s) : PData(s) { }
};
class CCPPointerInfo { public:
    long References;
    void* Object;
    void (*Destroy)(void*);
};
template <class T> class CCountedPointer {
public:
    T*              Data;
    CCPPointerInfo* Info;
    CCountedPointer() : Data(0), Info(0) { }
    CCountedPointer(const CCountedPointer& o) : Data(o.Data), Info(o.Info) { if (Info) ++Info->References; }
    void Release()
    {
        if (Info) {
            --Info->References;
            if (Info->References == 0) {
                Info->Destroy(Info->Object);
                delete Info;
            }
        }
    }
    ~CCountedPointer() { Release(); Data = 0; Info = 0; }
    T* operator->() const { return Data; }
};
class C2DVector {
public:
    float X, Y;
    C2DVector(float x, float y) : X(x), Y(y) {}
};
class CFontBank {
public:
    virtual ~CFontBank() { }
    virtual C2DVector GetTextExtent(const unsigned short* text, long flags)
    {
        CHECK(flags == 0);
        float w = 0;
        for (; *text; ++text) w += (*text == 'W') ? 13.75f : 7.5f;
        return C2DVector(w, 16.0f);
    }
};
static void DestroyBank(void* p) { ++g_bankDestroyed; delete (CFontBank*)p; }
class CFontManager {
public:
    CCountedPointer<CFontBank> GetFont(const CCharString& name)      // stub for 0x009E2C80: a fresh counted font
    {
        ++g_getFonts;
        std::strcpy(g_requestedFont, name.PData);
        CCountedPointer<CFontBank> p;
        p.Data = new CFontBank;
        p.Info = new CCPPointerInfo;
        p.Info->References = 1;
        p.Info->Object = p.Data;
        p.Info->Destroy = DestroyBank;
        return p;
    }
};
class CSystemManager {
public:
    CFontManager  Fonts;
    CFontManager* GetFontManager() const { return (CFontManager*)&Fonts; }
};
static CSystemManager g_system;
static CSystemManager* GFGetSystemManager() { return &g_system; }    // stub for 0x009A4EC0
class CText {
public:
    static unsigned int GetCharWidthW(unsigned short c)
    {
        CWideString text;
        text.Sprintf("%c", c);
        CCountedPointer<CFontBank> font = GFGetSystemManager()->GetFontManager()->GetFont("ENG_ARIAL_16");
        CFontBank* bank = font.Data;
        return (unsigned int)bank->GetTextExtent(text.GetAsCharArray(), 0).X;
    }
};
static void RunTest()
{
    CHECK(CText::GetCharWidthW('a') == 7);                // 7.5 truncated
    CHECK(CText::GetCharWidthW('W') == 13);               // 13.75 truncated
    CHECK(g_formats == 2 && g_getFonts == 2 && std::strcmp(g_requestedFont, "ENG_ARIAL_16") == 0);
    CHECK(g_bankDestroyed == 2);                          // the counted font is released each call
}

int main()
{
    RunTest();
    if (g_fail) {
        std::printf("0054f4b0_TEST FAIL\n");
        return 1;
    }
    std::printf("0054f4b0_TEST PASS\n");
    return 0;
}