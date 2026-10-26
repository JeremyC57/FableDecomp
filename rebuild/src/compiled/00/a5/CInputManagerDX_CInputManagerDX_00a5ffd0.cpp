// CInputManagerDX::~CInputManagerDX() -- DirectInput input manager teardown: delete the mouse and keyboard, empty the joystick vector, release the DirectInput object, then the CInputManager base destructor
// Leaf: ??1CInputManagerDX
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A5FFD0, length 119 bytes. Vtable 0x0129D620 (CInputManagerDX).
// Callees: the mouse / keyboard virtual deleting destructors (slot 0, flag 1), the CCountedPointer<CJoystick> range
// `_Move` @ 0x00A5FDA0 and `_Destroy` @ 0x009F6750 (both landed, `__fastcall` with empty tag arguments), the DirectInput
// object's IUnknown::Release (slot 2, `__stdcall`), CInputManager::~CInputManager @ 0x009F5C80.
// Layout: retail CInputManager (0x80 bytes): PMouse +0x24, PKeyboard +0x28, PJoysticks +0x2c; CInputManagerDX's
// DirectInput interface at +0x80.
// Shape: the scoped_ptr resets clear the member before the null test; the joystick `clear()` is the VC7.1
// `erase(begin(), end())` without its empty-range test, written as an inline member of the vector (in the body the
// last-pointer store sinks below the DirectInput load) -- as the landed CLipSyncDataBank::Close 0x00A42DA0: last then
// first read into locals, both empty tags homed in the high byte of the `push ecx` frame word).
struct SMoveTag { SMoveTag() { } };
struct SAllocatorTag { SAllocatorTag() { } };
struct SCountBlock;
struct CJoystickPointer {                          // CCountedPointer<CJoystick>: {object, count block}
    void*        m_pObject;
    SCountBlock* m_pCount;
};
CJoystickPointer* __fastcall Move_00a5fda0(CJoystickPointer* first, CJoystickPointer* last, CJoystickPointer* dest, const SMoveTag& tag, bool overlap);   // 0x00A5FDA0
void __fastcall Destroy_009f6750(CJoystickPointer* first, CJoystickPointer* last, const SAllocatorTag& allocator);   // 0x009F6750

class CMouse { public: virtual ~CMouse(); };
class CKeyboard { public: virtual ~CKeyboard(); };
struct CJoystickVector {                           // VC7.1 Dinkumware vector: first / last / end
    CJoystickPointer* m_first;
    CJoystickPointer* m_last;
    CJoystickPointer* m_end;
    void clear()                                   // erase(begin(), end()) without the empty-range test
    {
        CJoystickPointer* last = m_last;
        CJoystickPointer* first = m_first;
        CJoystickPointer* kept = Move_00a5fda0(last, last, first, SMoveTag(), false);
        Destroy_009f6750(kept, m_last, SAllocatorTag());
        m_last = kept;
    }
};
template <class T> struct scoped_ptr {
    T* m_p;
    void reset()
    {
        T* old = m_p;
        m_p = 0;
        if (old)
            delete old;
    }
};
struct IDirectInput8 {
    virtual long __stdcall QueryInterface(const void* iid, void** out);
    virtual unsigned long __stdcall AddRef();
    virtual unsigned long __stdcall Release();     // slot 2 (+0x08)
};

class CInputManager {
public:
    virtual ~CInputManager();                      // 0x009F5C80
    unsigned char           _pad04[0x20];          // +0x04 ProcessedEvents, EventStore, counters
    scoped_ptr<CMouse>      PMouse;                // +0x24
    scoped_ptr<CKeyboard>   PKeyboard;             // +0x28
    CJoystickVector         PJoysticks;            // +0x2c vector<CCountedPointer<CJoystick> >
    unsigned char           _pad38[0x48];
};
class CInputManagerDX : public CInputManager {
public:
    IDirectInput8*          m_pDirectInput;        // +0x80
    virtual ~CInputManagerDX();
};

CInputManagerDX::~CInputManagerDX()
{
    PMouse.reset();
    PKeyboard.reset();
    PJoysticks.clear();
    if (m_pDirectInput)
        m_pDirectInput->Release();
}