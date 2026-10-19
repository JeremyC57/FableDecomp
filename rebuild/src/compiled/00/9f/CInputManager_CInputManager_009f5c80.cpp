// CInputManager::~CInputManager() -- destroy the package file, the loaded event packages, the joysticks, the keyboard and mouse and both event vectors in reverse order, then the CBaseClass base
// Leaf: ??1CInputManager
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009F5C80, length 240 bytes. Called by the deleting destructor 0x009F6FC0
// (slot 0 of CInputManager's vtable 0x0129C79C) and 0x00A6003F; the catalog label
// "CPatchTesselationEdgeStrip::~CPatchTesselationEdgeStrip" is propagated.
// Callees: CDiskFileWin32::Close @ 0x0099A920, ~CWideString @ 0x0099B510, ~CBaseClassNonCopyable @ 0x0099A300 (the inline
//          CDiskFileWin32 destructor, vtable 0x0122D06C), the CInputEventPackage range destructor @ 0x009F6E00 and the
//          joystick-pointer range destructor @ 0x009F6750 (landed as Destroy_009f6750; both __fastcall first/last + tag),
//          the mouse's and keyboard's virtual deleting destructors (slot 0, flag 1), ~CInputEvent @ 0x00A03B30 (each element
//          of the two event vectors), free @ 0x00BFEA14 (x4), ~CBaseClass @ 0x0099A2E0.
// Layout: Ego_r.pdb CInputManager at retail offsets (12-byte vectors): ProcessedEvents +0x04, EventStore +0x10,
// NoStoredEvents +0x1c, NoEventScansRunning +0x20, PMouse +0x24, PKeyboard +0x28, PJoysticks +0x2c, Loading/Saving
// +0x38/+0x39, Checksum +0x3c, LoadedEventPackages +0x4c, CurrentFilePackageIndex +0x58, TimeStamp +0x5c, PackageFile
// +0x60 (CDiskFileWin32, as the landed CDefStringTable::SaveBinary model), NoUpdates +0x7c.
// Shape: members die in reverse order with every destructor inline: the file closes itself, the scoped pointers delete
// through the virtual destructor, the vectors destroy their range (out of line or as an element loop) and free.
extern "C" void __cdecl free(void* block);                  // 0x00BFEA14

class CBaseClass {
public:
    virtual void v00();
    ~CBaseClass();                                          // 0x0099A2E0
};
struct CBaseClassNonCopyable {
    virtual void v00();
    ~CBaseClassNonCopyable();                               // 0x0099A300
};
struct CWideString {
    void* PStringData;
    ~CWideString();                                         // 0x0099B510
};
struct CDiskFileWin32 : CBaseClassNonCopyable {             // vtable 0x0122D06C
    long        AccessType;                                 // +0x04
    long        Position;                                   // +0x08
    long        Length;                                     // +0x0c
    void*       WinFileHandle;                              // +0x10
    bool        Opened;                                     // +0x14
    CWideString Pathname;                                   // +0x18
    virtual ~CDiskFileWin32() { Close(); }
    virtual void Close();                                   // 0x0099A920
};

struct SAllocatorTag { SAllocatorTag() { } };

struct CInputEventPackage;
void __fastcall Destroy_009f6e00(CInputEventPackage* first, CInputEventPackage* last, const SAllocatorTag& allocator);   // 0x009F6E00
struct CJoystickPointer;
void __fastcall Destroy_009f6750(CJoystickPointer* first, CJoystickPointer* last, const SAllocatorTag& allocator);       // 0x009F6750

template<class T> struct CRangeVector {                     // vector whose elements are destroyed by an out-of-line range helper
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
};
struct CInputEventPackageVector : CRangeVector<CInputEventPackage> {
    ~CInputEventPackageVector()
    {
        Destroy_009f6e00(_M_start, _M_finish, SAllocatorTag());
        if (_M_start)
            free(_M_start);
    }
};
struct CJoystickPointerVector : CRangeVector<CJoystickPointer> {   // vector<CCountedPointer<CJoystick>>
    ~CJoystickPointerVector()
    {
        Destroy_009f6750(_M_start, _M_finish, SAllocatorTag());
        if (_M_start)
            free(_M_start);
    }
};

struct CInputEvent {                                        // 0x34 bytes
    unsigned char _body[0x34];
    ~CInputEvent();                                         // 0x00A03B30
};
template<class T> inline void DestroyRange(T* first, T* last)
{
    for (; first != last; ++first)
        first->~T();
}

struct CInputEventVector {
    CInputEvent* _M_start;
    CInputEvent* _M_finish;
    CInputEvent* _M_end_of_storage;
    ~CInputEventVector()
    {
        DestroyRange(_M_start, _M_finish);
        if (_M_start)
            free(_M_start);
    }
};

struct CMouse { virtual ~CMouse(); };
struct CKeyboard { virtual ~CKeyboard(); };
template<class T> struct scoped_ptr {
    T* ptr;
    ~scoped_ptr() { delete ptr; }
};

struct CChecksum { unsigned char _body[0x10]; };

class CInputManager : public CBaseClass {                   // vtable 0x0129C79C
public:
    CInputEventVector        ProcessedEvents;               // +0x04
    CInputEventVector        EventStore;                    // +0x10
    long                     NoStoredEvents;                // +0x1c
    long                     NoEventScansRunning;           // +0x20
    scoped_ptr<CMouse>       PMouse;                        // +0x24
    scoped_ptr<CKeyboard>    PKeyboard;                     // +0x28
    CJoystickPointerVector   PJoysticks;                    // +0x2c
    bool                     Loading;                       // +0x38
    bool                     Saving;                        // +0x39
    CChecksum                Checksum;                      // +0x3c
    CInputEventPackageVector LoadedEventPackages;           // +0x4c
    long                     CurrentFilePackageIndex;       // +0x58
    long                     TimeStamp;                     // +0x5c
    CDiskFileWin32           PackageFile;                   // +0x60
    unsigned long            NoUpdates;                     // +0x7c
    virtual ~CInputManager();
};

CInputManager::~CInputManager()
{
}