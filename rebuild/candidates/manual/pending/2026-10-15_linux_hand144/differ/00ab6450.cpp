// CKeyboardDX::SetExclusiveMode(bool exclusive) -- reset the keyboard for a cooperative-level change: drop the maintained events, unacquire, set the level (non-exclusive | foreground) on the main window, then re-evaluate the input focus; false when the level is refused
// Grade: DIFFER(131v131) -- register swap only: the window lands in eax and the device in ecx (retail: window ecx, device eax, vtable edx) for SetCooperativeLevel; tried window local, unsigned-long window, device local
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
    void* window = GFGetSystemManager()->MainWindow;
    if (DKeyboard->SetCooperativeLevel(window, 6) < 0)
        return false;
    if (GFGetSystemManager()->IsApplicationActive() && GFGetSystemManager()->HasInputFocus())
        SetHasInputFocus(true);
    else
        SetHasInputFocus(false);
    return true;
}
