// test for 00415940 CGameComponent scalar deleting destructor
#include <stdio.h>
#include <new>
class CGame;
class CBaseClassNonCopyable {
public:
    virtual ~CBaseClassNonCopyable();                    // 0x0099A300
};
class CDeviceResetCallback {
public:
    virtual bool OnPreDeviceReset() = 0;
    virtual bool OnPostDeviceReset() = 0;
};
class CGameComponent : public CBaseClassNonCopyable, public CDeviceResetCallback {
public:
    bool   Quit;                                         // +0x08
    bool   Running;                                      // +0x09
    CGame* Game;                                         // +0x0c
    CGameComponent(CGame* game) : Quit(false), Running(false), Game(game) {}
    virtual ~CGameComponent() {}
    virtual void Init();                                 // slot 1 (0x00415920, empty)
    virtual void Run() = 0;                              // slot 2 (__purecall; name descriptive)
    virtual void Shutdown() = 0;                         // slot 3 (__purecall; name descriptive)
protected:
    virtual void SetQuit();                              // slot 4 (0x00415930, empty)
};
class CGameComponent_Concrete : public CGameComponent {
public:
    CGameComponent_Concrete() : CGameComponent(0) {}
    virtual void Run();
    virtual void Shutdown();
    virtual bool OnPreDeviceReset();
    virtual bool OnPostDeviceReset();
};
void ConstructCGameComponent(void* p)                    // emission helper, not a retail function: CGameComponent's vtable (and its ??_G) is emitted
{
    new (p) CGameComponent_Concrete;
}

static int g_base; static void* g_basethis;
CBaseClassNonCopyable::~CBaseClassNonCopyable() { ++g_base; g_basethis = this; }
void CGameComponent::Init() {}
void CGameComponent::SetQuit() {}
void CGameComponent_Concrete::Run() {}
void CGameComponent_Concrete::Shutdown() {}
bool CGameComponent_Concrete::OnPreDeviceReset() { return true; }
bool CGameComponent_Concrete::OnPostDeviceReset() { return true; }
int main()
{
    static double buf[8];
    ConstructCGameComponent(buf);
    CGameComponent* p = (CGameComponent*)buf;
    int ok = p->Game == 0 && !p->Quit && !p->Running;
    p->~CGameComponent();
    ok = ok && g_base == 1 && g_basethis == (void*)buf;
    CGameComponent* h = new CGameComponent_Concrete;
    delete h;
    ok = ok && g_base == 2;
    printf(ok ? "00415940_TEST PASS\n" : "00415940_TEST FAIL\n");
    return 0;
}