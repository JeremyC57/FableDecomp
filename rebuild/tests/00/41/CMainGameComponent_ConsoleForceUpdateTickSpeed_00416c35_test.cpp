#include <cstdio>
class CGenericVar {
public:
    virtual ~CGenericVar();
    long          Type;                                  // +0x04 EGenericVarType
    void*         ValString;                             // +0x08 CCharString
    long          ValSLONG;                              // +0x0c
    unsigned long ValULONG;                              // +0x10
    bool          ValBool;                               // +0x14
    float         ValFloat;                              // +0x18
};
namespace std {
template <class T> class allocator {};
template <class T, class A = allocator<T> > class vector {
public:
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    const T& operator[](unsigned int n) const { return _M_start[n]; }
};
}
class CMainGameComponent {
public:
    virtual void _slot00();
    virtual void _slot01();
    virtual void _slot02();
    virtual void ChangeTextureColourDepth(long depth);   // slot 3
    virtual void SetQuit();                              // slot 4
    virtual void _slot05();
    virtual void _slot06();
    virtual void _slot07();
    virtual void PostInit();                             // slot 8
    virtual bool IsEditorActive() const;                 // slot 9 (+0x24)
    unsigned char _pad04[0x34 - 0x4];
    bool          ForceUpdateTick;                       // +0x34
    float         ForceUpdateTickSpeedMultipler;         // +0x38
    float         ForceUpdateTickSpeedDesiredFramerate;  // +0x3c
    long          ForceUpdateNoFailedUpdates;            // +0x40
protected:
    static void __fastcall ConsoleForceUpdateTickSpeed(CMainGameComponent& component, const std::vector<CGenericVar>& args);
    friend struct CMainGameComponent_Test;               // behaviour-test hook
};
void __fastcall CMainGameComponent::ConsoleForceUpdateTickSpeed(CMainGameComponent& component, const std::vector<CGenericVar>& args)
{
    bool force = args[0].ValBool;
    float multiplier = args[1].ValFloat;
    float framerate = args[2].ValFloat;
    if (component.IsEditorActive())
        return;
    component.ForceUpdateTick = force;
    component.ForceUpdateTickSpeedMultipler = multiplier;
    component.ForceUpdateTickSpeedDesiredFramerate = framerate;
    if (force)
    {
        component.ForceUpdateNoFailedUpdates = 0;
        if (multiplier < 0.1f)
            component.ForceUpdateTickSpeedMultipler = 1.0f;
    }
}

CGenericVar::~CGenericVar() {}
void CMainGameComponent::_slot00() {} void CMainGameComponent::_slot01() {} void CMainGameComponent::_slot02() {}
void CMainGameComponent::ChangeTextureColourDepth(long) {} void CMainGameComponent::SetQuit() {}
void CMainGameComponent::_slot05() {} void CMainGameComponent::_slot06() {} void CMainGameComponent::_slot07() {}
void CMainGameComponent::PostInit() {}
static bool g_editor;
bool CMainGameComponent::IsEditorActive() const { return g_editor; }
struct CMainGameComponent_Test {
    static void Run(CMainGameComponent& c, const std::vector<CGenericVar>& a) { CMainGameComponent::ConsoleForceUpdateTickSpeed(c, a); }
};
int main()
{
    CMainGameComponent c; c.ForceUpdateTick = false; c.ForceUpdateTickSpeedMultipler = 7.0f; c.ForceUpdateTickSpeedDesiredFramerate = 7.0f; c.ForceUpdateNoFailedUpdates = 9;
    CGenericVar v[3]; v[0].ValBool = true; v[1].ValFloat = 2.0f; v[2].ValFloat = 30.0f;
    std::vector<CGenericVar> a; a._M_start = v; a._M_finish = a._M_end_of_storage = v + 3;
    g_editor = true; CMainGameComponent_Test::Run(c, a);
    bool ok = c.ForceUpdateTick == false && c.ForceUpdateTickSpeedMultipler == 7.0f && c.ForceUpdateNoFailedUpdates == 9;
    g_editor = false; CMainGameComponent_Test::Run(c, a);
    ok = ok && c.ForceUpdateTick && c.ForceUpdateTickSpeedMultipler == 2.0f && c.ForceUpdateTickSpeedDesiredFramerate == 30.0f && c.ForceUpdateNoFailedUpdates == 0;
    v[1].ValFloat = 0.05f; c.ForceUpdateNoFailedUpdates = 4; CMainGameComponent_Test::Run(c, a);
    ok = ok && c.ForceUpdateTickSpeedMultipler == 1.0f && c.ForceUpdateNoFailedUpdates == 0;
    v[0].ValBool = false; c.ForceUpdateNoFailedUpdates = 4; CMainGameComponent_Test::Run(c, a);
    ok = ok && !c.ForceUpdateTick && c.ForceUpdateTickSpeedMultipler == 0.05f && c.ForceUpdateNoFailedUpdates == 4;
    std::printf(ok ? "00416c35_TEST PASS\n" : "00416c35_TEST FAIL\n");
    return ok ? 0 : 1;
}