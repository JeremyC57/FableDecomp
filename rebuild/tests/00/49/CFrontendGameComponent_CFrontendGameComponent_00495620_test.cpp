#include <cstdio>
extern "C" void __cdecl free(void* p);                     // 0x00BFEA14
class CBaseClassNonCopyable {
protected:
    virtual ~CBaseClassNonCopyable();                                      // 0x0099A300
};
struct CGameComponentInterface {
    virtual void v00();
};
class CGameComponent : public CBaseClassNonCopyable, public CGameComponentInterface { public:   // vtables 0x0122ECB0 / 0x0122ECA4
    bool m_bShutDown;                                      // +0x08
    virtual __forceinline ~CGameComponent() { }
};
class CCPPointerInfo { public:
    long  References;
    void  (__fastcall* Destroy)(void* object);
    void* Object;
};
template <class T> class CCountedPointer { public:
    T*              Data;
    CCPPointerInfo* Info;
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
};
class CWideString { public:
    void* PStringData;
    ~CWideString();                                        // 0x0099B510
};
struct CWideStringVector {                                 // STLport vector<CWideString>
    CWideString* _M_start;
    CWideString* _M_finish;
    CWideString* _M_end_of_storage;
    static __forceinline void _Destroy(CWideString* first, CWideString* last)
    {
        for (; first != last; ++first)
            first->~CWideString();
    }
    ~CWideStringVector()
    {
        _Destroy(_M_start, _M_finish);
        if (_M_start)
            free(_M_start);
    }
};
class CTexture { public:
    unsigned char _data[8];
    ~CTexture();                                           // 0x009F9F70
};
class CGraphicDataBank; class CFontBank;
enum ECursorMovement { CURSOR_MOVEMENT_NONE = 0 };
struct CFrontendGameComponent : CGameComponent {           // vtables 0x01238768 / 0x0123875C
    unsigned char                     _pad00c[0x1dfc - 0x0c];
    CCountedPointer<CGraphicDataBank> PTempGraphics;              // +0x1dfc
    CCountedPointer<const CFontBank>  MainFont;                   // +0x1e04
    CWideStringVector                 ProfileNames;               // +0x1e0c
    long                              ProfileNamesOffset;         // +0x1e18
    CWideStringVector                 SaveNames;                  // +0x1e1c
    long                              SaveNamesOffset;            // +0x1e28
    CWideString                       SaveToLoad;                 // +0x1e2c
    CWideStringVector                 WorldNames;                 // +0x1e30
    long                              WorldNamesOffset;           // +0x1e3c
    CWideString                       WorldToLoad;                // +0x1e40
    long                              CursorHighlight;            // +0x1e44
    double                            TimeLastMovementHappened;   // +0x1e48
    bool                              StartedRepeatingMovement;   // +0x1e50
    ECursorMovement                   CurrentMovement;            // +0x1e54
    CTexture                          SplashScreen;               // +0x1e58
    virtual ~CFrontendGameComponent();
};

CFrontendGameComponent::~CFrontendGameComponent()
{
}

#include <stdlib.h>
#include <string.h>
#include <new>
static int g_fail = 0;
static void check(bool c, const char* w) { if (!c) { printf("FAIL %s\n", w); g_fail = 1; } }
static int g_strDtors = 0, g_texDtors = 0, g_baseDtors = 0, g_destroyed = 0;
static char g_order[256];
CBaseClassNonCopyable::~CBaseClassNonCopyable() { ++g_baseDtors; strcat(g_order, "Base;"); }
void CGameComponentInterface::v00() {}
CWideString::~CWideString() { ++g_strDtors; char t[16]; sprintf(t, "S%ld;", (long)PStringData); strcat(g_order, t); }
CTexture::~CTexture() { ++g_texDtors; strcat(g_order, "Tex;"); }
static void __fastcall DestroyObj(void* o) { ++g_destroyed; strcat(g_order, (const char*)o); }
static void FillVector(CWideStringVector& v, int n, long base)
{
    v._M_start = n ? (CWideString*)malloc(n * sizeof(CWideString)) : 0;
    for (int i = 0; i < n; ++i) v._M_start[i].PStringData = (void*)(base + i);
    v._M_finish = v._M_start + n; v._M_end_of_storage = v._M_finish;
}
static CCPPointerInfo* MakeInfo(long refs, const char* tag)
{
    CCPPointerInfo* i = new CCPPointerInfo; i->References = refs; i->Destroy = DestroyObj; i->Object = (void*)tag; return i;
}
int main()
{
    void* mem = malloc(sizeof(CFrontendGameComponent));
    memset(mem, 0, sizeof(CFrontendGameComponent));
    CFrontendGameComponent* c = new (mem) CFrontendGameComponent;
    FillVector(c->ProfileNames, 2, 10); FillVector(c->SaveNames, 0, 20); FillVector(c->WorldNames, 3, 30);
    c->SaveToLoad.PStringData = (void*)2; c->WorldToLoad.PStringData = (void*)3;
    c->MainFont.Info = MakeInfo(1, "Font;"); c->MainFont.Data = (CFontBank*)1;
    CCPPointerInfo* shared = MakeInfo(2, "Temp;");
    c->PTempGraphics.Info = shared; c->PTempGraphics.Data = (CGraphicDataBank*)1;
    c->~CFrontendGameComponent();
    check(strcmp(g_order, "Tex;S3;S30;S31;S32;S2;S10;S11;Font;Base;") == 0, "destruction order");
    check(g_strDtors == 7 && g_texDtors == 1 && g_baseDtors == 1 && g_destroyed == 1, "counts");
    check(shared->References == 1, "shared pointer only decremented");
    check(c->MainFont.Data == 0 && c->MainFont.Info == 0 && c->PTempGraphics.Data == 0 && c->PTempGraphics.Info == 0, "pointers cleared");
    if (!g_fail) printf("00495620_TEST PASS\n");
    else printf("%s\n", g_order);
    return g_fail;
}