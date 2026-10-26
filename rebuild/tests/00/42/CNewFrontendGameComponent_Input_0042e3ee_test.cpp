// Behaviour test for 0042e3ee CNewFrontendGameComponent::Input: while the input-delay timer runs the queue is cleared;
// otherwise each queued event is forwarded to the front-end UI manager (key / char / mouse / wheel / joystick), the
// key, stick and pad buttons gather direction flags, and the flags then fire one UI command (with the 0.333 s initial
// and 0.085 s repeat delay of the repeat timer).
#include <cstdio>
class C2DVector { public: float X, Y; };
class CInputEvent { public:
    long Type, Key, Char, Button, Device; float Wheel; C2DVector MousePosition; C2DVector Stick;
    long GetType() const { return Type; }
    long GetDeviceNumber() const { return Device; }
    long GetKey() const { return Key; }
    unsigned short GetChar() const { return (unsigned short)Char; }
    long GetJoystickButton() const { return Button; }
    float GetMouseWheel() const { return Wheel; }
    C2DVector GetJoystickPosition() const { return Stick; }
};
class CInputEventScanInfo { public: bool Started; long Index; CInputEventScanInfo() : Started(false), Index(0) {} };
class CInputManager { public:
    CInputEvent* events; long count; int cleared; int updates; int scansEnded;
    void Update() { ++updates; }
    bool EventScanStart(CInputEventScanInfo& s, CInputEvent& e) { s.Started = true; s.Index = 0; if (count <= 0) return false; e = events[0]; return true; }
    bool EventScanNext(CInputEventScanInfo& s, CInputEvent& e) { if (++s.Index >= count) return false; e = events[s.Index]; return true; }
    void EventScanEnd(CInputEventScanInfo& s) { if (s.Started) ++scansEnded; }
    void ClearEvents() { ++cleared; count = 0; }
};
static CInputManager g_input;
struct CManager {
    long commands[32]; int n; long keys[8]; int nk; C2DVector MousePosition;
    void SendCommand(long c) { if (n < 32) commands[n++] = c; }
    void KeyPressed(unsigned short k) { if (nk < 8) keys[nk++] = k; }
};
static CManager g_ui;
static CManager* GetInstance() { return &g_ui; }
static int g_consoleKeys;
class CConsole { public: void ProcessInput(const CInputEvent&) { ++g_consoleKeys; } };
static CConsole g_console;
struct CActiveJoystick { long DeviceNumber; bool Locked; bool IsLocked() const { return Locked; } long GetDeviceNumber() const { return DeviceNumber; } };
static CActiveJoystick g_pad;
static float g_elapsed;
struct CStopWatch {
    bool m_bIsRunning; int starts, stops, resets;
    void StartZero() { m_bIsRunning = true; ++starts; }
    void Stop() { m_bIsRunning = false; ++stops; }
    void Reset() { ++resets; }
    float GetElapsedSeconds() const { return g_elapsed; }
    bool IsRunning() const { return m_bIsRunning; }
};
static const float g_epsilon = 0.0001f;
class CNewFrontendGameComponent { public:
    bool m_bAllowInputs; CStopWatch m_RepeatTimer; float m_fRepeatDelay; unsigned long m_dwDPadTriggerState;
    CStopWatch m_attractModeTimer; CStopWatch m_inputDelayTimer; long LastActiveJoystickDeviceNumber;
    void Input()
    {
        if (m_inputDelayTimer.IsRunning()) { g_input.ClearEvents(); return; }
        CInputEventScanInfo scan; CInputEvent event;
        g_input.Update();
        unsigned long flags = 0;
        for (bool more = g_input.EventScanStart(scan, event); more; more = g_input.EventScanNext(scan, event)) {
            switch (event.GetType()) {
            case 1:
                g_console.ProcessInput(event);
                if (event.GetKey() == 0x6f) flags |= 4;
                if (event.GetKey() == 0x70) flags |= 8;
                if (event.GetKey() == 0x6d) flags |= 1;
                if (event.GetKey() == 0x72) flags |= 2;
                if (event.GetKey() == 0x1e) flags |= 0x100;
                if (event.GetKey() == 0x30) flags |= 0x200;
                if (event.GetKey() == 0x15) flags |= 0x20000;
                GetInstance()->KeyPressed((unsigned short)event.GetKey());
                GetInstance()->SendCommand(0x21);
                break;
            case 4: GetInstance()->SendCommand(0x1a); break;
            case 6: GetInstance()->SendCommand(0x1c); break;
            case 7: GetInstance()->SendCommand(0x23); break;
            case 10: GetInstance()->SendCommand(0x1b); break;
            case 13: { C2DVector p = event.MousePosition; GetInstance()->MousePosition = p; GetInstance()->SendCommand(0x19); break; }
            case 14:
                if (event.GetMouseWheel() > g_epsilon) GetInstance()->SendCommand(0x24);
                else if (event.GetMouseWheel() < -g_epsilon) GetInstance()->SendCommand(0x25);
                break;
            case 15: GetInstance()->KeyPressed(event.GetChar()); GetInstance()->SendCommand(0x22); break;
            case 17:
                if (m_bAllowInputs) {
                    C2DVector s = event.GetJoystickPosition();
                    if (s.Y > 0.5f) flags |= 2; else if (s.Y < -0.5f) flags |= 1;
                    if (s.X > 0.5f) flags |= 8; else if (s.X < -0.5f) flags |= 4;
                }
                break;
            case 19: {
                bool activePad = true;
                if (g_pad.IsLocked()) activePad = g_pad.GetDeviceNumber() == event.GetDeviceNumber();
                if (m_bAllowInputs && activePad) {
                    LastActiveJoystickDeviceNumber = event.GetDeviceNumber();
                    switch (event.GetJoystickButton()) {
                    case 1: flags |= 0x10000; break; case 2: flags |= 0x20000; break;
                    case 4: flags |= 0x100; break;   case 5: flags |= 0x200; break;
                    case 11: flags |= 0x400; break;  case 12: flags |= 0x800; break;
                    case 13: m_dwDPadTriggerState |= 0x10; break; case 14: m_dwDPadTriggerState |= 0x20; break;
                    case 15: m_dwDPadTriggerState |= 0x40; break; case 16: m_dwDPadTriggerState |= 0x80; break;
                    }
                }
                break;
            }
            case 21:
                if (m_bAllowInputs) {
                    switch (event.GetJoystickButton()) {
                    case 13: m_dwDPadTriggerState &= ~0x10UL; break; case 14: m_dwDPadTriggerState &= ~0x20UL; break;
                    case 15: m_dwDPadTriggerState &= ~0x40UL; break; case 16: m_dwDPadTriggerState &= ~0x80UL; break;
                    }
                }
                break;
            case 22: GetInstance()->SendCommand(0x26); break;
            case 23: GetInstance()->SendCommand(0x27); break;
            case 24: GetInstance()->SendCommand(0x28); break;
            case 25: GetInstance()->SendCommand(0x29); break;
            case 26: GetInstance()->SendCommand(0x2a); break;
            }
        }
        g_input.EventScanEnd(scan);
        if (m_dwDPadTriggerState) flags |= m_dwDPadTriggerState;
        if (flags) {
            m_attractModeTimer.Reset();
            if (m_RepeatTimer.IsRunning()) {
                if (m_RepeatTimer.GetElapsedSeconds() < m_fRepeatDelay) return;
                m_fRepeatDelay = 0.085f; m_RepeatTimer.StartZero();
            } else { m_fRepeatDelay = 0.333f; m_RepeatTimer.StartZero(); }
        } else { m_fRepeatDelay = 0.333f; m_RepeatTimer.Stop(); }
        if (flags & 0x400) GetInstance()->SendCommand(4);
        else if (flags & 0x800) GetInstance()->SendCommand(5);
        else if (flags & 0x100) GetInstance()->SendCommand(4);
        else if (flags & 0x200) GetInstance()->SendCommand(5);
        else if (flags & 0x20000) GetInstance()->SendCommand(0x16);
        else if (flags & 4 || flags & 0x40) { GetInstance()->SendCommand(2); GetInstance()->SendCommand(0x14); }
        else if (flags & 8 || flags & 0x80) { GetInstance()->SendCommand(3); GetInstance()->SendCommand(0x15); }
        else if (flags & 1 || flags & 0x10) GetInstance()->SendCommand(0);
        else if (flags & 2 || flags & 0x20) GetInstance()->SendCommand(1);
        else if (flags & 0x1000) GetInstance()->SendCommand(8);
        else if (flags & 0x2000) GetInstance()->SendCommand(9);
        else if (flags & 0x4000) GetInstance()->SendCommand(0xa);
        else if (flags & 0x8000) GetInstance()->SendCommand(0xb);
        else if (flags & 0x10000) GetInstance()->SendCommand(0x17);
    }
};
static CInputEvent Ev(long type) { CInputEvent e; e.Type = type; e.Key = 0; e.Char = 0; e.Button = 0; e.Device = 0; e.Wheel = 0.0f; e.MousePosition.X = 0; e.MousePosition.Y = 0; e.Stick.X = 0; e.Stick.Y = 0; return e; }
static void Fresh(CNewFrontendGameComponent& c)
{
    c.m_bAllowInputs = true; c.m_RepeatTimer.m_bIsRunning = false; c.m_RepeatTimer.starts = c.m_RepeatTimer.stops = c.m_RepeatTimer.resets = 0;
    c.m_fRepeatDelay = 0.0f; c.m_dwDPadTriggerState = 0; c.m_attractModeTimer.m_bIsRunning = false; c.m_attractModeTimer.resets = 0;
    c.m_inputDelayTimer.m_bIsRunning = false; c.LastActiveJoystickDeviceNumber = -1;
    g_ui.n = 0; g_ui.nk = 0; g_input.cleared = 0; g_input.updates = 0; g_input.scansEnded = 0; g_consoleKeys = 0;
}
static int Cmds(long a, long b, long c) { long w[3] = { a, b, c }; int n = 0; for (int i = 0; i < 3; ++i) if (w[i] >= 0) ++n; if (g_ui.n != n) return 0; for (int i = 0; i < n; ++i) if (g_ui.commands[i] != w[i]) return 0; return 1; }
int main()
{
    int ok = 1;
    CNewFrontendGameComponent c; CInputEvent q[8];
    /* input delay running: the queue is dropped */
    Fresh(c); c.m_inputDelayTimer.m_bIsRunning = true; q[0] = Ev(1); g_input.events = q; g_input.count = 1;
    c.Input();
    if (g_input.cleared != 1 || g_input.updates != 0 || g_ui.n != 0) ok = 0;
    /* key 0x6f (left): key + 0x21 to the UI, then left = 2, 0x14; the repeat timer starts with 0.333 */
    Fresh(c); q[0] = Ev(1); q[0].Key = 0x6f; g_input.events = q; g_input.count = 1;
    c.Input();
    if (!Cmds(0x21, 2, 0x14) || g_ui.keys[0] != 0x6f || g_consoleKeys != 1 || g_input.scansEnded != 1) ok = 0;
    if (!c.m_RepeatTimer.m_bIsRunning || c.m_fRepeatDelay != 0.333f || c.m_attractModeTimer.resets != 1) ok = 0;
    /* held again before the delay: nothing fires */
    g_ui.n = 0; g_elapsed = 0.1f; g_input.count = 1; c.Input();
    if (!Cmds(0x21, -1, -1)) ok = 0;
    /* after the delay: fires and switches to the 0.085 repeat */
    g_ui.n = 0; g_elapsed = 0.5f; g_input.count = 1; c.Input();
    if (!Cmds(0x21, 2, 0x14) || c.m_fRepeatDelay != 0.085f) ok = 0;
    /* no flags: the repeat timer stops */
    g_ui.n = 0; g_input.count = 0; c.Input();
    if (c.m_RepeatTimer.m_bIsRunning || c.m_fRepeatDelay != 0.333f || g_ui.n != 0) ok = 0;
    /* mouse move, wheel up/down, char */
    Fresh(c); q[0] = Ev(13); q[0].MousePosition.X = 3; q[0].MousePosition.Y = 4;
    q[1] = Ev(14); q[1].Wheel = 1.0f; q[2] = Ev(14); q[2].Wheel = -1.0f; q[3] = Ev(14);
    q[4] = Ev(15); q[4].Char = 'a'; g_input.events = q; g_input.count = 5;
    c.Input();
    if (g_ui.n != 4 || g_ui.commands[0] != 0x19 || g_ui.commands[1] != 0x24 || g_ui.commands[2] != 0x25 || g_ui.commands[3] != 0x22 || g_ui.keys[0] != 'a' || g_ui.MousePosition.X != 3 || g_ui.MousePosition.Y != 4) ok = 0;
    /* pad: wrong device while locked is ignored; right device dpad-down held (0x20 -> 1) */
    Fresh(c); g_pad.Locked = true; g_pad.DeviceNumber = 2;
    q[0] = Ev(19); q[0].Device = 1; q[0].Button = 11; q[1] = Ev(19); q[1].Device = 2; q[1].Button = 14;
    g_input.events = q; g_input.count = 2; c.Input();
    if (!Cmds(1, -1, -1) || c.LastActiveJoystickDeviceNumber != 2 || c.m_dwDPadTriggerState != 0x20) ok = 0;
    /* dpad released */
    g_ui.n = 0; q[0] = Ev(21); q[0].Button = 14; g_input.count = 1; c.Input();
    if (c.m_dwDPadTriggerState != 0 || g_ui.n != 0) ok = 0;
    /* stick right + button 11 (priority 4) */
    Fresh(c); g_pad.Locked = false; q[0] = Ev(17); q[0].Stick.X = 0.9f; q[1] = Ev(19); q[1].Button = 11; g_input.count = 2; c.Input();
    if (!Cmds(4, -1, -1)) ok = 0;
    /* stick right alone: 3, 0x15; disallowed inputs ignore stick */
    Fresh(c); q[0] = Ev(17); q[0].Stick.X = 0.9f; g_input.count = 1; c.Input();
    if (!Cmds(3, 0x15, -1)) ok = 0;
    Fresh(c); c.m_bAllowInputs = false; g_input.count = 1; c.Input();
    if (g_ui.n != 0) ok = 0;
    /* plain commands and button 1 -> 0x17 */
    Fresh(c); q[0] = Ev(4); q[1] = Ev(26); q[2] = Ev(19); q[2].Button = 1; g_input.count = 3; c.Input();
    if (!Cmds(0x1a, 0x2a, 0x17)) ok = 0;
    if (!ok) { std::printf("0042e3ee_TEST FAIL\n"); return 1; }
    std::printf("0042e3ee_TEST PASS\n");
    return 0;
}