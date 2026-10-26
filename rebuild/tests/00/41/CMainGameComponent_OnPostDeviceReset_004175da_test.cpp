#include <stdio.h>
static int g_fonts; static const void* g_this;
struct CBaseClassNonCopyable { virtual ~CBaseClassNonCopyable() {} };
class CGameComponent : public CBaseClassNonCopyable { public: };
class CDeviceResetCallback { public:
    virtual bool OnPreDeviceReset() = 0;
    virtual bool OnPostDeviceReset() = 0;
};
class CMainGameComponent : public CGameComponent, public CDeviceResetCallback { public:
    void InitialiseFonts() { ++g_fonts; g_this = this; }
    virtual bool OnPreDeviceReset() { return false; }
    virtual bool OnPostDeviceReset();
};

bool CMainGameComponent::OnPostDeviceReset()
{
    InitialiseFonts();
    return true;
}

int main()
{
    CMainGameComponent c;
    CDeviceResetCallback* cb = &c;
    bool r = cb->OnPostDeviceReset();
    bool ok = r && g_fonts == 1 && g_this == &c && (void*)cb != (void*)&c;
    ok = ok && cb->OnPostDeviceReset() && g_fonts == 2;
    printf(ok ? "004175da_TEST PASS\n" : "004175da_TEST FAIL\n");
    return ok ? 0 : 1;
}