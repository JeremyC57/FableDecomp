// CInputProcessMain::IsEventFromPlayerDevice(const CInputEvent& event) -- a joystick event (device 1) counts only when the player uses that joystick device number; every other device is accepted
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00687DB0, length 48 bytes. Vtable 0x0123758C slot 8 (the chunk name "vfunc_8").
// Callees: CInputEvent::GetDevice @ 0x00A03B50 (+0x20), CInputEvent::GetDeviceNumber @ 0x00A03B60 (+0x24; both labelled "CWorld::DrawGetEnvironment"),
// CPlayer::IsUsingJoystickDeviceNumber(long) @ 0x004874F0 (MyPlayer +0x18, engine/CInputProcessGameBase.h).
// Shape: the event reference in edi for both accessor calls, `cmp eax,1 ; jne` to the `mov al,1` tail, the bool of the player call
// passed through al, `ret 4`.
struct CInputEvent {
    long GetDevice() const;                         // 0x00A03B50
    long GetDeviceNumber() const;                   // 0x00A03B60
};
struct CPlayer {
    bool IsUsingJoystickDeviceNumber(long deviceNumber);   // 0x004874F0
};
struct CInputProcessGameBase {                      // engine/CInputProcessGameBase.h layout
    virtual void v00();
    unsigned char _pad04[0x10];                     // +0x04 CAInputProcess
    void*         World;                            // +0x14
    CPlayer*      MyPlayer;                         // +0x18
    void*         DefinitionManager;                // +0x1c
    void*         Component;                        // +0x20
    void*         PGamePlayerInterface;             // +0x24
};
struct CInputProcessMain : CInputProcessGameBase {  // vtable 0x0123758C
    bool IsEventFromPlayerDevice(const CInputEvent& event);
};

bool CInputProcessMain::IsEventFromPlayerDevice(const CInputEvent& event)
{
    if (event.GetDevice() == 1)
        return MyPlayer->IsUsingJoystickDeviceNumber(event.GetDeviceNumber());
    return true;
}