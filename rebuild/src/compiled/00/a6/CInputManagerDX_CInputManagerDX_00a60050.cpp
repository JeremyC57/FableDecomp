// CInputManagerDX::CInputManagerDX(const CInputManagerInit& init, bool exclusive) -- build the generic input manager, create the DirectInput8 interface on the application's instance handle, then create the DirectX joystick (kept in the joystick list), keyboard and mouse devices the init record asks for
// Leaf: ??0CInputManagerDX@@QAE@ABVCInputManagerInit@@_N@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A60050, 333 bytes, `ret 8`. Vtable 0x0129D620.
// Identity: Ego_r.pdb / FableWin.pdb ??0CInputManagerDX@@QAE@ABVCInputManagerInit@@_N@Z (bound in reloc_bindings.tsv for this
//   address); the devices' ctors are ??0CJoystickDX@@QAE@AAVCInputManagerDX@@_N@Z / ??0CKeyboardDX@@... / ??0CMouseDX@@...
//   (both PDBs: a manager REFERENCE and the same bool, here forwarded as `push ebx ; push esi`).
// Callees: CInputManager::CInputManager(const CInputManagerInit&, bool) @ 0x009F5BD0, GFGetSystemManager @ 0x009A4EC0
//   (?GFGetSystemManager@@YIPAVCSystemManager@@XZ, both PDBs), DirectInput8Create @ 0x00BFEFF8 (import thunk;
//   IID_IDirectInput8W @ 0x012ACBF4), operator new @ 0x00BFEA1A, CJoystickDX @ 0x00AB7850 (0xD44 bytes), CKeyboardDX @
//   0x00AB64E0 (0x3614 bytes), CMouseDX @ 0x00AB5D00 (0x4860 bytes), CCountedPointer<CJoystick>(CJoystick*) @ 0x00A5FC30
//   (retail keeps this instantiation out of line), the joystick vector's _M_insert_overflow @ 0x00A5FEF0, operator delete @
//   0x00BFE9BC (count block), the devices' deleting destructors (slot 0).
// Layout: Ego_r.pdb CInputManager with retail 12-byte STLport vectors (ProcessedEvents +0x04, EventStore +0x10,
//   NoStoredEvents +0x1C, NoEventScansRunning +0x20, PMouse +0x24, PKeyboard +0x28, PJoysticks +0x2C; PDB +0x2C..+0x34);
//   CInputManagerDX::DInput +0x80 (PDB +0x90); CSystemManager::WinInstanceHandle +0x90 (PDB +0x88). CInputManagerInit is the
//   PDB's three bools (UseMouse +0, UseJoystick +1, UseKeyboard +2). RTTI bases (ghidra_out/rtti_bases.tsv): CInputManager :
//   CBaseClass, CJoystickDX : CJoystick (no base), CKeyboardDX : CKeyboard : CBaseClassNonCopyable, CMouseDX : CMouse :
//   CBaseClassNonCopyable. engine/CJoystick.h, CJoystickDX.h, CKeyboard.h and CMouse.h flatten the vptr (generated, no
//   virtuals), so the device classes are local models with a virtual destructor only; no CInputManager / CSystemManager header.
// Shape: the joystick goes through a counted-pointer temporary (out-of-line constructor, inline STLport push_back copy and
//   the inline release); keyboard and mouse are scoped_ptr::reset (old pointer read, new one stored, old one deleted).
//   The instance handle is read through the PDB accessor CSystemManager::GetWinInstanceHandle() (QAEPAUHINSTANCE__@@XZ):
//   with a direct member read VC7.1 reloads the joystick count block for operator delete into eax; through the inline
//   accessor it uses ecx, as retail.
#include "engine/CBaseClassNonCopyable.h"
#include "engine/CCountedPointer.h"
struct _GUID { unsigned long Data1; unsigned short Data2, Data3; unsigned char Data4[8]; };
typedef _GUID GUID;
extern "C" const GUID IID_IDirectInput8W;          // 0x012ACBF4
struct IDirectInput8W;
struct IUnknown;
typedef struct HINSTANCE__* HINSTANCE;
extern "C" long __stdcall DirectInput8Create(HINSTANCE instance, unsigned long version, const GUID& iid, void** out, IUnknown* outer);   // 0x00BFEFF8
class CSystemManager { public:
    unsigned char _pad[0x90];
    HINSTANCE     WinInstanceHandle;               // +0x90
    HINSTANCE GetWinInstanceHandle() { return WinInstanceHandle; }
};
CSystemManager* __fastcall GFGetSystemManager();   // 0x009A4EC0
class CInputManagerInit { public:
    bool UseMouse;                                 // +0
    bool UseJoystick;                              // +1
    bool UseKeyboard;                              // +2
};
inline void* operator new(unsigned int, void* where) { return where; }
class CJoystick;
namespace std {
struct __false_type { __false_type() { } };
template <class T> class allocator;
template <class T, class A> class vector;
template <> class vector<CCountedPointer<CJoystick>, allocator<CCountedPointer<CJoystick> > > {
public:
    typedef CCountedPointer<CJoystick> T;
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    void push_back(const T& x)
    {
        if (_M_finish != _M_end_of_storage) {
            new (_M_finish) T(x);
            ++_M_finish;
        } else
            _M_insert_overflow(_M_finish, x, __false_type(), 1, true);
    }
protected:
    void _M_insert_overflow(T* position, const T& x, const __false_type&, unsigned int fill_len, bool atend);   // 0x00A5FEF0
};
}
template <class T> class scoped_ptr { public:
    T* ptr;
    void reset(T* p)
    {
        T* old = ptr;
        ptr = p;
        delete old;
    }
};
class CInputManagerDX;
class CJoystick { public:
    virtual ~CJoystick();
};
class CMouse : public CBaseClassNonCopyable { };
class CKeyboard : public CBaseClassNonCopyable { };
class CJoystickDX : public CJoystick { public: CJoystickDX(CInputManagerDX& manager, bool exclusive); unsigned char _body[0xd44 - 4]; };   // 0x00AB7850
class CKeyboardDX : public CKeyboard { public: CKeyboardDX(CInputManagerDX& manager, bool exclusive); unsigned char _body[0x3614 - 4]; }; // 0x00AB64E0
class CMouseDX : public CMouse { public: CMouseDX(CInputManagerDX& manager, bool exclusive); unsigned char _body[0x4860 - 4]; };          // 0x00AB5D00
class CInputManager : public CBaseClass {
public:
    CInputManager(const CInputManagerInit& init, bool exclusive);   // 0x009F5BD0
    unsigned char                            _pad04[0x24 - 0x04];   // ProcessedEvents, EventStore, NoStoredEvents, NoEventScansRunning
    scoped_ptr<CMouse>                       PMouse;        // +0x24
    scoped_ptr<CKeyboard>                    PKeyboard;     // +0x28
    std::vector<CCountedPointer<CJoystick>, std::allocator<CCountedPointer<CJoystick> > > PJoysticks;   // +0x2c
    unsigned char                            _pad38[0x80 - 0x38];
};
class CInputManagerDX : public CInputManager {   // vtable 0x0129D620
public:
    IDirectInput8W* DInput;                        // +0x80
    CInputManagerDX(const CInputManagerInit& init, bool exclusive);
    virtual ~CInputManagerDX();
};

CInputManagerDX::CInputManagerDX(const CInputManagerInit& init, bool exclusive)
    : CInputManager(init, exclusive), DInput(0)
{
    HINSTANCE instance = GFGetSystemManager()->GetWinInstanceHandle();
    DirectInput8Create(instance, 0x800, IID_IDirectInput8W, (void**)&DInput, 0);
    if (init.UseJoystick)
        PJoysticks.push_back(CCountedPointer<CJoystick>(new CJoystickDX(*this, exclusive)));
    if (init.UseKeyboard)
        PKeyboard.reset(new CKeyboardDX(*this, exclusive));
    if (init.UseMouse)
        PMouse.reset(new CMouseDX(*this, exclusive));
}