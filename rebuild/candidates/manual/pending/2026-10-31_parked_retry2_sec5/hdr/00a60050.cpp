// CInputManagerDX::CInputManagerDX(const SInputDeviceConfig& config, HWND window) -- build the generic input manager, create the DirectInput8 interface on the application instance, then create the DirectX joystick (kept in the joystick list), keyboard and mouse devices the config asks for
// Leaf: ??0CInputManagerDX
// Grade: DIFFER(333v333) -- local CCountedPointer replaced by engine/CCountedPointer.h (bytes unchanged: the (T*) ctor
//   stays the out-of-line 0x00A5FC30 call, the dead zero stores vanish); residue: the count-block delete reloads Info
//   into eax (retail ecx); a named joystick local is worse (348)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A60050, length 333 bytes, `ret 8`. Vtable 0x0129D620.
// Callees: CInputManager::CInputManager(const SInputDeviceConfig&, HWND) @ 0x009F5BD0, GFGetSystemManager @ 0x009A4EC0
//   (application instance at +0x90), DirectInput8Create @ 0x00BFEFF8 (import thunk; IID_IDirectInput8W @ 0x012ACBF4),
//   operator new @ 0x00BFEA1A, CJoystickDX(CInputManagerDX*, HWND) @ 0x00AB7850 (0xD44 bytes), CKeyboardDX @ 0x00AB64E0
//   (0x3614 bytes), CMouseDX @ 0x00AB5D00 (0x4860 bytes), CCountedPointer<CJoystick>(CJoystick*) @ 0x00A5FC30 (out of
//   line; manifest "CCountedPointer<CFileInstaller::CFileInfo const>" is a folded twin), the joystick vector's
//   _M_insert_overflow @ 0x00A5FEF0, operator delete @ 0x00BFE9BC (count blocks), the devices' deleting destructors (slot 0).
// Layout: Ego_r.pdb CInputManager with retail 12-byte STLport vectors (ProcessedEvents +0x04, EventStore +0x10, so PMouse
//   +0x24, PKeyboard +0x28, PJoysticks +0x2C, 16 bytes below the PDB's +0x2C..+0x34 chain); CInputManagerDX::DInput +0x80
//   (PDB +0x90). The config is three bools: mouse +0, joystick +1, keyboard +2. No engine headers; modelled locally.
// Shape: the joystick goes through a counted-pointer temporary (out-of-line constructor, inline push_back copy and inline
//   release); keyboard and mouse are scoped_ptr::reset (old pointer read, new one stored, old one deleted).
struct GUID_Retail { unsigned long Data1; unsigned short Data2, Data3; unsigned char Data4[8]; };
extern const GUID_Retail IID_IDirectInput8W;       // 0x012ACBF4
struct IDirectInput8W;
struct IUnknown;
typedef struct HWND__* HWND;
typedef struct HINSTANCE__* HINSTANCE;
extern "C" long __stdcall DirectInput8Create(HINSTANCE instance, unsigned long version, const GUID_Retail& iid, void** out, IUnknown* outer);   // 0x00BFEFF8
struct CSystemManager {
    unsigned char _pad[0x90];
    HINSTANCE     Instance;                        // +0x90
};
CSystemManager* __fastcall GFGetSystemManager();   // 0x009A4EC0
struct SInputDeviceConfig {
    bool UseMouse;                                 // +0
    bool UseJoystick;                              // +1
    bool UseKeyboard;                              // +2
};
#include "engine/CCountedPointer.h"
inline void* operator new(unsigned int, void* where) { return where; }
struct __false_type { __false_type() { } };
template <class T> struct CRetailVector {
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    void _M_insert_overflow(T* position, const T& x, const __false_type&, unsigned int fill, bool atEnd);   // 0x00A5FEF0
    void push_back(const T& x)
    {
        if (_M_finish != _M_end_of_storage) {
            new (_M_finish) T(x);
            ++_M_finish;
        } else
            _M_insert_overflow(_M_finish, x, __false_type(), 1, true);
    }
};
template <class T> struct scoped_ptr {
    T* ptr;
    void reset(T* p)
    {
        T* old = ptr;
        ptr = p;
        delete old;
    }
};
class CInputManagerDX;
struct CInputDevice {
    virtual ~CInputDevice();
};
struct CJoystick : CInputDevice { };
struct CMouse : CInputDevice { };
struct CKeyboard : CInputDevice { };
struct CJoystickDX : CJoystick { CJoystickDX(CInputManagerDX* manager, HWND window); unsigned char _body[0xd44 - 4]; };   // 0x00AB7850
struct CKeyboardDX : CKeyboard { CKeyboardDX(CInputManagerDX* manager, HWND window); unsigned char _body[0x3614 - 4]; };  // 0x00AB64E0
struct CMouseDX : CMouse { CMouseDX(CInputManagerDX* manager, HWND window); unsigned char _body[0x4860 - 4]; };           // 0x00AB5D00
class CInputManager {
public:
    CInputManager(const SInputDeviceConfig& config, HWND window);   // 0x009F5BD0
    virtual ~CInputManager();
    unsigned char                            _pad04[0x24 - 0x04];
    scoped_ptr<CMouse>                       PMouse;        // +0x24
    scoped_ptr<CKeyboard>                    PKeyboard;     // +0x28
    CRetailVector<CCountedPointer<CJoystick> > PJoysticks;  // +0x2c
    unsigned char                            _pad38[0x80 - 0x38];
};
class CInputManagerDX : public CInputManager {   // vtable 0x0129D620
public:
    IDirectInput8W* DInput;                        // +0x80
    CInputManagerDX(const SInputDeviceConfig& config, HWND window);
    virtual ~CInputManagerDX();
};

CInputManagerDX::CInputManagerDX(const SInputDeviceConfig& config, HWND window)
    : CInputManager(config, window), DInput(0)
{
    HINSTANCE instance = GFGetSystemManager()->Instance;
    DirectInput8Create(instance, 0x800, IID_IDirectInput8W, (void**)&DInput, 0);
    if (config.UseJoystick)
        PJoysticks.push_back(CCountedPointer<CJoystick>(new CJoystickDX(this, window)));
    if (config.UseKeyboard)
        PKeyboard.reset(new CKeyboardDX(this, window));
    if (config.UseMouse)
        PMouse.reset(new CMouseDX(this, window));
}
