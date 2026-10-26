#include <cstdio>
class CCharString {
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
class CCPPointerInfo { public:                                 // CCountedPointer count block
    long References;                                    // +0
    void (__fastcall* Destroy)(void* object);           // +4
    void* Object;                                       // +8
};
template <class T> class CCountedPointer { public:
    T*              Data;                               // +0
    CCPPointerInfo* Info;                               // +4
    void Release()
    {
        if (Info) {
            --Info->References;
            if (Info->References == 0) {
                Info->Destroy(Info->Object);
                operator delete(Info);
            }
        }
    }
    ~CCountedPointer() { Release(); Data = 0; Info = 0; }
    CCountedPointer& operator=(const CCountedPointer& other)
    {
        T* data = other.Data;
        CCPPointerInfo* info = other.Info;
        if (Info != info) {
            Release();
            Data = data;
            Info = info;
            if (info)
                ++info->References;
        }
        return *this;
    }
};
class CFontBank {
public:
    CCountedPointer<const CFontBank> GetFont(const CCharString& name);   // 0x009E2C80
};
extern CFontBank* g_pFontBank;                          // 0x013B838C

class CFrontendGameComponent {
public:
    unsigned char                    _pad0000[0x1e04];
    CCountedPointer<const CFontBank> MainFont;          // +0x1e04
    bool InitGraphics();
};

bool CFrontendGameComponent::InitGraphics()
{
    MainFont = g_pFontBank->GetFont("ENG_ARIAL_16");
    return true;
}

#include <string.h>
static int g_fail = 0;
static void check(bool c, const char* w) { if (!c) { printf("FAIL %s\n", w); g_fail = 1; } }
static char g_name[64]; static long g_len = 0; static int g_strDtors = 0, g_destroyed = 0;
CCharString::CCharString(const char* text, long length) { strcpy(g_name, text); g_len = length; PStringData = 0; }
CCharString::~CCharString() { ++g_strDtors; }
static void __fastcall DestroyFont(void* object) { ++g_destroyed; }
static CFontBank g_bank;
CFontBank* g_pFontBank = &g_bank;
static CCPPointerInfo* g_lastInfo;
struct RawPointer { const CFontBank* Data; CCPPointerInfo* Info; };   // POD storage: no stub-local destructor runs
static RawPointer g_raw;
CCountedPointer<const CFontBank> CFontBank::GetFont(const CCharString& name)
{
    g_raw.Data = this;
    g_raw.Info = new CCPPointerInfo;
    g_raw.Info->References = 1;             // the returned temporary's reference
    g_raw.Info->Destroy = DestroyFont;
    g_raw.Info->Object = this;
    g_lastInfo = g_raw.Info;
    return *reinterpret_cast<CCountedPointer<const CFontBank>*>(&g_raw);
}
int main()
{
    static CFrontendGameComponent c;
    // first call: empty MainFont takes the font; the temporary's reference is dropped (count 1 left)
    check(c.InitGraphics() == true, "returns true");
    check(strcmp(g_name, "ENG_ARIAL_16") == 0 && g_len == -1, "font name");
    check(c.MainFont.Data == &g_bank && c.MainFont.Info == g_lastInfo, "assigned");
    check(c.MainFont.Info->References == 1 && g_destroyed == 0, "one reference left");
    check(g_strDtors == 1, "name destroyed");
    // second call: the old font's last reference is released (destroyed once), the new one is held
    CCPPointerInfo* first = c.MainFont.Info;
    check(c.InitGraphics() == true, "returns true again");
    check(g_destroyed == 1, "old font destroyed");
    check(c.MainFont.Info == g_lastInfo && c.MainFont.Info != first && c.MainFont.Info->References == 1, "new font held");
    if (!g_fail) printf("00494840_TEST PASS\n");
    return g_fail;
}