// Behaviour test for 00ab6450: src/00ab6450.cpp copied verbatim; the DirectInput device, the system manager and the
// event list stubbed so the clear/unacquire/cooperative-level sequence, the failure return and the focus decision are checked.
#include <cstdio>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// CKeyboardDX::SetExclusiveMode(bool exclusive) -- reset the keyboard for a cooperative-level change: drop the maintained events, unacquire, set the level (non-exclusive | foreground) on the main window, then re-evaluate the input focus; false when the level is refused
// Retry 10b (getsw sweep, FIXES B-15): the main window is read through an inline getter `GetMainWindow()`; the plain member read gives the parked register/scheduling difference.
// Leaf: ?SetExclusiveMode@CKeyboardDX@@UAE_N_N@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AB6450, length 131 bytes.
// Callees: list<CMaintainedEvent>::clear() @ 0x009E4710 on the list at +0x340c (manifest "CGuiControlTreePane::SortTreeRecursively"
// is propagated; landed as unattributed_clearCRetailList), the IDirectInputDevice8 COM slots 8 (+0x20, Unacquire) and 13
// (+0x34, SetCooperativeLevel(window, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND = 6)), GFGetSystemManager @ 0x009A4EC0 (the main
// window at +0x94), CSystemManager::IsApplicationActive @ 0x009A57B0 / HasInputFocus @ 0x009A4FB0, the keyboard's own
// virtual SetHasInputFocus (slot 3).
// Model: retail CKeyboardDX layout (maintained-event list +0x340c, device +0x3418, Acquired +0x3610); the focus test as the
// landed CKeyboardDX::UpdateAcquired 0x00AB6100. The argument is unused (the level is always 6).
// Shape: the main window is read into a local first (inline, the device and its vtable are fetched after the call).
struct IDirectInputDevice8_Retail {                // COM: __stdcall methods, `this` pushed
    virtual long __stdcall QueryInterface(void* iid, void** out);
    virtual unsigned long __stdcall AddRef();
    virtual unsigned long __stdcall Release();
    virtual long __stdcall GetCapabilities(void* caps);
    virtual long __stdcall EnumObjects(void* callback, void* ref, unsigned long flags);
    virtual long __stdcall GetProperty(const void* guid, void* header);
    virtual long __stdcall SetProperty(const void* guid, const void* header);
    virtual long __stdcall Acquire();              // slot 7 (+0x1c)
    virtual long __stdcall Unacquire();            // slot 8 (+0x20)
    virtual long __stdcall GetDeviceState(unsigned long size, void* data);   // slot 9 (+0x24)
    virtual long __stdcall GetDeviceData(unsigned long size, void* data, unsigned long* count, unsigned long flags);   // slot 10 (+0x28)
    virtual long __stdcall SetDataFormat(const void* format);   // slot 11 (+0x2c)
    virtual long __stdcall SetEventNotification(void* event);   // slot 12 (+0x30)
    virtual long __stdcall SetCooperativeLevel(void* window, unsigned long flags);   // slot 13 (+0x34)
    virtual long __stdcall GetObjectInfo(void* info, unsigned long object, unsigned long how);   // slot 14 (+0x38)
    virtual long __stdcall GetDeviceInfo(void* info);   // slot 15 (+0x3c)
};
struct CSystemManager {
    unsigned char _pad[0x94];
    void*         MainWindow;                      // +0x94 (HWND)
    void* GetMainWindow() const { return MainWindow; }
    bool IsApplicationActive();                    // 0x009A57B0
    bool HasInputFocus();                          // 0x009A4FB0
};
CSystemManager* __cdecl GFGetSystemManager();      // 0x009A4EC0
struct CMaintainedEventList {                      // STLport list<CMaintainedEvent>
    void* m_pNode;
    void clear();                                  // 0x009E4710
};
class CKeyboardDX {
public:
    virtual void v000(); virtual void v001(); virtual void v002();
    virtual void SetHasInputFocus(bool focus);     // slot 3 (+0xc)
    virtual bool SetExclusiveMode(bool exclusive);
    unsigned char               _pad04[0x340c - 4];
    CMaintainedEventList        MaintainedEventsList;   // +0x340c
    unsigned char               _pad3410[8];
    IDirectInputDevice8_Retail* DKeyboard;         // +0x3418
    unsigned char               KeyStates[0x100];  // +0x341c
    unsigned short              KeyMap[0x7a];      // +0x351c
    bool                        Acquired;          // +0x3610
};

bool CKeyboardDX::SetExclusiveMode(bool exclusive)
{
    MaintainedEventsList.clear();
    Acquired = false;
    DKeyboard->Unacquire();
    void* window = GFGetSystemManager()->GetMainWindow();
    if (DKeyboard->SetCooperativeLevel(window, 6) < 0)
        return false;
    if (GFGetSystemManager()->IsApplicationActive() && GFGetSystemManager()->HasInputFocus())
        SetHasInputFocus(true);
    else
        SetHasInputFocus(false);
    return true;
}
// ---- end source ----
static int g_clears = 0, g_unacquires = 0, g_coops = 0; static void* g_coopWindow = 0; static unsigned long g_coopFlags = 0;
static long g_coopResult = 0; static bool g_active = true, g_focus = true; static int g_focusCalls = 0; static bool g_focusArg = false;
long __stdcall IDirectInputDevice8_Retail::QueryInterface(void*, void**) { return 0; }
unsigned long __stdcall IDirectInputDevice8_Retail::AddRef() { return 1; }
unsigned long __stdcall IDirectInputDevice8_Retail::Release() { return 0; }
long __stdcall IDirectInputDevice8_Retail::GetCapabilities(void*) { return 0; }
long __stdcall IDirectInputDevice8_Retail::EnumObjects(void*, void*, unsigned long) { return 0; }
long __stdcall IDirectInputDevice8_Retail::GetProperty(const void*, void*) { return 0; }
long __stdcall IDirectInputDevice8_Retail::SetProperty(const void*, const void*) { return 0; }
long __stdcall IDirectInputDevice8_Retail::Acquire() { return 0; }
long __stdcall IDirectInputDevice8_Retail::Unacquire() { ++g_unacquires; return 0; }
long __stdcall IDirectInputDevice8_Retail::GetDeviceState(unsigned long, void*) { return 0; }
long __stdcall IDirectInputDevice8_Retail::GetDeviceData(unsigned long, void*, unsigned long*, unsigned long) { return 0; }
long __stdcall IDirectInputDevice8_Retail::SetDataFormat(const void*) { return 0; }
long __stdcall IDirectInputDevice8_Retail::SetEventNotification(void*) { return 0; }
long __stdcall IDirectInputDevice8_Retail::SetCooperativeLevel(void* w, unsigned long f) { ++g_coops; g_coopWindow = w; g_coopFlags = f; return g_coopResult; }
long __stdcall IDirectInputDevice8_Retail::GetObjectInfo(void*, unsigned long, unsigned long) { return 0; }
long __stdcall IDirectInputDevice8_Retail::GetDeviceInfo(void*) { return 0; }
static CSystemManager g_sys;
CSystemManager* __cdecl GFGetSystemManager() { return &g_sys; }
bool CSystemManager::IsApplicationActive() { return g_active; }
bool CSystemManager::HasInputFocus() { return g_focus; }
void CMaintainedEventList::clear() { ++g_clears; }
void CKeyboardDX::v000() {} void CKeyboardDX::v001() {} void CKeyboardDX::v002() {}
void CKeyboardDX::SetHasInputFocus(bool focus) { ++g_focusCalls; g_focusArg = focus; }
static CKeyboardDX g_kb;
int main()
{
    static IDirectInputDevice8_Retail dev;
    g_sys.MainWindow = (void*)0x1234;
    g_kb.DKeyboard = &dev; g_kb.Acquired = true;
    CHECK(g_kb.SetExclusiveMode(true) == true);
    CHECK(g_clears == 1 && !g_kb.Acquired && g_unacquires == 1 && g_coops == 1 && g_coopWindow == (void*)0x1234 && g_coopFlags == 6);
    CHECK(g_focusCalls == 1 && g_focusArg == true);
    g_focus = false;
    CHECK(g_kb.SetExclusiveMode(false) == true && g_focusCalls == 2 && g_focusArg == false);
    g_coopResult = -1;
    CHECK(g_kb.SetExclusiveMode(true) == false && g_focusCalls == 2 && g_clears == 3);
    if (fails == 0) printf("00ab6450_TEST PASS\n"); else printf("00ab6450_TEST FAIL\n");
    return fails;
}