// CNewFrontendGameComponent::Input() -- drain the input manager's event queue into the front-end UI manager (keys, characters, mouse move/wheel, joystick buttons and stick), then turn the gathered direction/button flags into repeating UI commands
// Leaf: ?Input@CNewFrontendGameComponent@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0042E3EE, 1441 bytes, frame pointer, `ret`.
// Layout: Ego_r.pdb CNewFrontendGameComponent (336 bytes) with the two retail 12-byte STLport vectors (FileNames,
//   FileTimes) => members after +0x6c sit 8 lower: m_RepeatTimer +0xd8 (running flag +0xf0), m_fRepeatDelay +0xf8,
//   m_dwDPadTriggerState +0xfc, m_attractModeTimer +0x100, m_inputDelayTimer +0x120 (running flag +0x138),
//   LastActiveJoystickDeviceNumber +0x140. m_bAllowInputs +0x28 is before the vectors (unshifted).
// Shape: size TU with a frame pointer (`#pragma optimize("s", on)` + ("y", off)): compare-tree switches, the flag word lives in [ebp-4], ebx/esi pushed after the early exit. The direction tests are written `a || b` so VC7.1 merges them into one `test bl,mask` and places the arms below the chain as retail does; the character arm passes an unsigned short (KeyPressed takes the UI character), which reorders the push after `mov ecx,esi`.
#pragma optimize("s", on)
#pragma optimize("y", off)
struct C2DVector {
    float X, Y;
    C2DVector() {}
    C2DVector(const C2DVector& v) : X(v.X), Y(v.Y) {}
};
class CInputEvent {
public:
    unsigned char _pad00[0x0c];
    C2DVector     MousePosition;                   // +0x0c (inside UInputEventData)
    unsigned char _pad14[0x34 - 0x14];
    CInputEvent();                                 // 0x00A04410
    ~CInputEvent();                                // 0x00A03B30
    long GetType() const;                          // 0x00A03B40
    long GetDeviceNumber() const;                  // 0x00A03B60
    long GetKey() const;                           // 0x00A03B70
    unsigned short GetChar() const;                         // 0x00A03B80
    long GetJoystickButton() const;                // 0x00A03B90
    float GetMouseWheel() const;                   // 0x00A03BB0
    C2DVector GetJoystickPosition() const;         // 0x00A043A0
};
struct CInputEventScanInfo {
    bool Started;
    long Index;
    CInputEventScanInfo() : Started(false) {}
};
class CInputManager {
public:
    virtual void v00();
    virtual void v04();
    virtual void Update();                         // slot 2
    bool EventScanStart(CInputEventScanInfo& scan, CInputEvent& event);   // 0x009F4ED0
    bool EventScanNext(CInputEventScanInfo& scan, CInputEvent& event);    // 0x009F4F10
    void EventScanEnd(CInputEventScanInfo& scan);                         // 0x009F4AC0
    void ClearEvents();                                                   // 0x009F5540
};
extern CInputManager* g_pInputManager;             // 0x013B8388
namespace NUISystem {
class CManager {
public:
    virtual void SendCommand(long command);        // slot 0
    virtual void v04(); virtual void v08(); virtual void v0c(); virtual void v10(); virtual void v14();
    virtual void v18(); virtual void v1c(); virtual void v20(); virtual void v24(); virtual void v28(); virtual void v2c();
    virtual void KeyPressed(unsigned short key);             // slot 12 (+0x30)
    unsigned char _pad04[0xb0 - 4];
    C2DVector     MousePosition;                   // +0xb0
    static CManager* GetInstance();                // 0x0041E5F2
};
}
class CConsole {
public:
    void ProcessInput(const CInputEvent& event);   // 0x009EA530
};
template <class T> class CTBaseSingleton {
public:
    static T& __fastcall Get();                    // 0x00414C90 (CConsole)
};
class CActiveJoystick {
public:
    unsigned char _pad00[4];
    long DeviceNumber;                             // +4
    bool Locked;                                   // +8
    long GetDeviceNumber() const;                  // 0x0049BB40
    bool IsLocked() const;                         // 0x0049BB50
    static CActiveJoystick& __fastcall Get();      // 0x0049BB90
};
class CStopWatch {
public:
    float     m_fTimerPeriod;
    long      _pad04;
    unsigned long m_nStartTick[2];
    unsigned long m_nPrevElapsedTicks[2];
    bool      m_bIsRunning;                        // +0x18
    unsigned char _pad19[7];
    void StartZero();                              // 0x0062F800
    void Stop();                                   // 0x0062F850
    void Reset();                                  // 0x0062F8B0
    float GetElapsedSeconds() const;               // 0x0062F900
    bool IsRunning() const { return m_bIsRunning; }
};
extern const float g_epsilon_0129ba3c;             // 0x0129BA3C (1e-4f, the shared float epsilon)
class CNewFrontendGameComponent {
public:
    virtual void v00();
    unsigned char _pad04[0x28 - 4];
    bool          m_bAllowInputs;                  // +0x28
    unsigned char _pad29[0xd8 - 0x29];
    CStopWatch    m_RepeatTimer;                   // +0xd8
    float         m_fRepeatDelay;                  // +0xf8
    unsigned long m_dwDPadTriggerState;            // +0xfc
    CStopWatch    m_attractModeTimer;              // +0x100
    CStopWatch    m_inputDelayTimer;               // +0x120
    long          LastActiveJoystickDeviceNumber;  // +0x140
    virtual void Input();
};

void CNewFrontendGameComponent::Input()
{
    if (m_inputDelayTimer.IsRunning()) {
        g_pInputManager->ClearEvents();
        return;
    }
    CInputEventScanInfo scan;
    CInputEvent event;
    CInputEvent lastEvent;
    g_pInputManager->Update();
    unsigned long flags = 0;
    for (bool more = g_pInputManager->EventScanStart(scan, event); more; more = g_pInputManager->EventScanNext(scan, event)) {
        switch (event.GetType()) {
        case 1:
            CTBaseSingleton<CConsole>::Get().ProcessInput(event);
            if (event.GetKey() == 0x6f) flags |= 4;
            if (event.GetKey() == 0x70) flags |= 8;
            if (event.GetKey() == 0x6d) flags |= 1;
            if (event.GetKey() == 0x72) flags |= 2;
            if (event.GetKey() == 0x1e) flags |= 0x100;
            if (event.GetKey() == 0x30) flags |= 0x200;
            if (event.GetKey() == 0x15) flags |= 0x20000;
            NUISystem::CManager::GetInstance()->KeyPressed(event.GetKey());
            NUISystem::CManager::GetInstance()->SendCommand(0x21);
            break;
        case 4:
            NUISystem::CManager::GetInstance()->SendCommand(0x1a);
            break;
        case 6:
            NUISystem::CManager::GetInstance()->SendCommand(0x1c);
            break;
        case 7:
            NUISystem::CManager::GetInstance()->SendCommand(0x23);
            break;
        case 10:
            NUISystem::CManager::GetInstance()->SendCommand(0x1b);
            break;
        case 13: {
            C2DVector position = event.MousePosition;
            NUISystem::CManager::GetInstance()->MousePosition = position;
            NUISystem::CManager::GetInstance()->SendCommand(0x19);
            break;
        }
        case 14:
            if (event.GetMouseWheel() > g_epsilon_0129ba3c)
                NUISystem::CManager::GetInstance()->SendCommand(0x24);
            else if (event.GetMouseWheel() < -g_epsilon_0129ba3c)
                NUISystem::CManager::GetInstance()->SendCommand(0x25);
            break;
        case 15:
            NUISystem::CManager::GetInstance()->KeyPressed(event.GetChar());
            NUISystem::CManager::GetInstance()->SendCommand(0x22);
            break;
        case 17:
            if (m_bAllowInputs) {
                C2DVector stick = event.GetJoystickPosition();
                if (stick.Y > 0.5f) flags |= 2;
                else if (stick.Y < -0.5f) flags |= 1;
                if (stick.X > 0.5f) flags |= 8;
                else if (stick.X < -0.5f) flags |= 4;
            }
            break;
        case 19: {
            bool activePad = true;
            if (CActiveJoystick::Get().IsLocked())
                activePad = CActiveJoystick::Get().GetDeviceNumber() == event.GetDeviceNumber();
            if (m_bAllowInputs && activePad) {
                LastActiveJoystickDeviceNumber = event.GetDeviceNumber();
                switch (event.GetJoystickButton()) {
                case 1:  flags |= 0x10000; break;
                case 2:  flags |= 0x20000; break;
                case 4:  flags |= 0x100; break;
                case 5:  flags |= 0x200; break;
                case 11: flags |= 0x400; break;
                case 12: flags |= 0x800; break;
                case 13: m_dwDPadTriggerState |= 0x10; break;
                case 14: m_dwDPadTriggerState |= 0x20; break;
                case 15: m_dwDPadTriggerState |= 0x40; break;
                case 16: m_dwDPadTriggerState |= 0x80; break;
                }
            }
            break;
        }
        case 21:
            if (m_bAllowInputs) {
                switch (event.GetJoystickButton()) {
                case 13: m_dwDPadTriggerState &= ~0x10; break;
                case 14: m_dwDPadTriggerState &= ~0x20; break;
                case 15: m_dwDPadTriggerState &= ~0x40; break;
                case 16: m_dwDPadTriggerState &= ~0x80; break;
                }
            }
            break;
        case 22:
            NUISystem::CManager::GetInstance()->SendCommand(0x26);
            break;
        case 23:
            NUISystem::CManager::GetInstance()->SendCommand(0x27);
            break;
        case 24:
            NUISystem::CManager::GetInstance()->SendCommand(0x28);
            break;
        case 25:
            NUISystem::CManager::GetInstance()->SendCommand(0x29);
            break;
        case 26:
            NUISystem::CManager::GetInstance()->SendCommand(0x2a);
            break;
        }
    }
    g_pInputManager->EventScanEnd(scan);
    if (m_dwDPadTriggerState)
        flags |= m_dwDPadTriggerState;
    if (flags) {
        m_attractModeTimer.Reset();
        if (m_RepeatTimer.IsRunning()) {
            if (m_RepeatTimer.GetElapsedSeconds() < m_fRepeatDelay)
                return;
            m_fRepeatDelay = 0.085f;
            m_RepeatTimer.StartZero();
        } else {
            m_fRepeatDelay = 0.333f;
            m_RepeatTimer.StartZero();
        }
    } else {
        m_fRepeatDelay = 0.333f;
        m_RepeatTimer.Stop();
    }
    if (flags & 0x400)
        NUISystem::CManager::GetInstance()->SendCommand(4);
    else if (flags & 0x800)
        NUISystem::CManager::GetInstance()->SendCommand(5);
    else if (flags & 0x100)
        NUISystem::CManager::GetInstance()->SendCommand(4);
    else if (flags & 0x200)
        NUISystem::CManager::GetInstance()->SendCommand(5);
    else if (flags & 0x20000)
        NUISystem::CManager::GetInstance()->SendCommand(0x16);
    else if (flags & 4 || flags & 0x40) {
        NUISystem::CManager::GetInstance()->SendCommand(2);
        NUISystem::CManager::GetInstance()->SendCommand(0x14);
    } else if (flags & 8 || flags & 0x80) {
        NUISystem::CManager::GetInstance()->SendCommand(3);
        NUISystem::CManager::GetInstance()->SendCommand(0x15);
    } else if (flags & 1 || flags & 0x10)
        NUISystem::CManager::GetInstance()->SendCommand(0);
    else if (flags & 2 || flags & 0x20)
        NUISystem::CManager::GetInstance()->SendCommand(1);
    else if (flags & 0x1000)
        NUISystem::CManager::GetInstance()->SendCommand(8);
    else if (flags & 0x2000)
        NUISystem::CManager::GetInstance()->SendCommand(9);
    else if (flags & 0x4000)
        NUISystem::CManager::GetInstance()->SendCommand(0xa);
    else if (flags & 0x8000)
        NUISystem::CManager::GetInstance()->SendCommand(0xb);
    else if (flags & 0x10000)
        NUISystem::CManager::GetInstance()->SendCommand(0x17);
}
