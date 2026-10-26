// CJoystickDX::`scalar deleting destructor'(unsigned int flags) -- this class's vtable (0x0129EA7C), the device (+0xd38) unacquired (COM slot 8), released (slot 2) and cleared when present, then CJoystick's destructor (0x009E4420: its vtable, the maintained-event list) and the flag-tested operator delete
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AB7950, length 76 bytes.
// Leaf: ??_GCJoystickDX@@UAEPAXI@Z
// Callees: the device's COM slots, ~CJoystick @ 0x009E4420 (labelled "JoystickGui_Destructor"), operator delete @ 0x00BFE9BC. Vtable slot 9. Shape: the device load before the vtable store, `this` in esi, `ret 4`.
void __cdecl operator delete(void* block);         // 0x00BFE9BC
struct IDirectInputDevice8_Retail {              // COM: __stdcall methods, `this` pushed
    virtual long __stdcall QueryInterface(void* iid, void** out);   // slot 0
    virtual unsigned long __stdcall AddRef();                       // slot 1
    virtual unsigned long __stdcall Release();                      // slot 2 (+0x08)
    virtual long __stdcall GetCapabilities(void* caps);             // slot 3
    virtual long __stdcall EnumObjects(void* callback, void* ref, unsigned long flags);   // slot 4
    virtual long __stdcall GetProperty(void* guid, void* header);   // slot 5
    virtual long __stdcall SetProperty(void* guid, void* header);   // slot 6
    virtual long __stdcall Acquire();                               // slot 7 (+0x1c)
    virtual long __stdcall Unacquire();                             // slot 8 (+0x20)
};
class CJoystick { public:                                 // retail layout 0xd18 bytes (vtable 0x0129C0EC)
    virtual void EventScanStart();                 // slot 0
    virtual void v001();
    virtual void v002();
    virtual void v003();
    virtual void v004();
    virtual void v005();
    virtual void v006();
    virtual void v007();
    virtual void v008();
    virtual ~CJoystick();                          // slot 9, 0x009E4420
    unsigned char _body[0xd18 - 4];
};
class CInputManagerDX;
struct CJoystickDX : CJoystick {                   // vtable 0x0129EA7C
    unsigned char               _pad0d18[0x20];    // +0xd18 the axis values and ranges
    IDirectInputDevice8_Retail* DJoystick;         // +0xd38
    bool                        Acquired;          // +0xd3c
    unsigned char               _pad0d3d[3];
    CInputManagerDX*            InputManagerDX;    // +0xd40
    __forceinline virtual ~CJoystickDX()
    {
        if (DJoystick) {
            DJoystick->Unacquire();
            DJoystick->Release();
            DJoystick = 0;
        }
    }
};

// VC7.1 emits a class's vtable (and the deleting destructor it references) in every translation
// unit that constructs the object; this helper is that construction, nothing more.
void* MakeOne_00ab7950()
{
    return new CJoystickDX;
}