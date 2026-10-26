// CKeyboardDX::`scalar deleting destructor'(unsigned int flags) -- this class's vtable (0x0129EA60), the DirectInput keyboard (+0x3418) unacquired (COM slot 8) and released (slot 2), then CKeyboard's destructor (0x00A66990: its vtable, the maintained-event list) and the flag-tested operator delete
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AB6A50, length 64 bytes.
// Leaf: ??_GCKeyboardDX@@UAEPAXI@Z
// Callees: the device's COM slots (no null test), ~CKeyboard @ 0x00A66990 (labelled "CBase_Destructor_Helper"), operator delete @ 0x00BFE9BC. Vtable slot 0. Shape: the device load before the vtable store, `this` in esi, `ret 4`.
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
class CKeyboard { public:                                 // retail layout 0x3418 bytes (vtable 0x0129DBA8)
    virtual ~CKeyboard();                          // slot 0, 0x00A66990
    unsigned char _body[0x3418 - 4];
};
struct CKeyboardDX : CKeyboard {                   // vtable 0x0129EA60
    IDirectInputDevice8_Retail* DKeyboard;         // +0x3418
    unsigned char               KeyStates[0x100];  // +0x341c
    __forceinline virtual ~CKeyboardDX()
    {
        DKeyboard->Unacquire();
        DKeyboard->Release();
    }
};

// VC7.1 emits a class's vtable (and the deleting destructor it references) in every translation
// unit that constructs the object; this helper is that construction, nothing more.
void* MakeOne_00ab6a50()
{
    return new CKeyboardDX;
}