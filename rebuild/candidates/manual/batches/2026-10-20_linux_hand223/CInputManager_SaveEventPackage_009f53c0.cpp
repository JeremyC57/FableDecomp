// CInputManager::SaveEventPackage() -- if any events are stored: sample the mouse position and the first joystick's axes, refresh the internal checksum, and write a package of the stored events (time stamp, checksum, both positions) to the package file
// Leaf: SaveEventPackage
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009F53C0, length 261 bytes (+ int3 padding), plain `ret`.
// Callees: CalculateInternalChecksum @ 0x009F5170 (long), STLport vector<CInputEvent> copy ctor @ 0x009F66A0,
//   CInputEventPackage::Save(CAFile&) @ 0x009F52A0, ~CInputEvent @ 0x00A03B30, free @ 0x00BFEA14 (the STLport
//   allocator's deallocate); CMouse virtual slot 1 (+0x04, position into a C2DVector), CJoystick virtual slots 3/4
//   (+0x0c/+0x10, axis floats).
// Layout (retail, 12-byte STLport vectors; Ego_r PDB names shifted): EventStore +0x10, NoStoredEvents +0x1c,
//   PMouse +0x24, PJoysticks +0x2c (CCountedPointer<CJoystick>, pointer first), CChecksum Checksum +0x3c (its 4th word
//   at +0x48 takes the internal checksum), TimeStamp +0x5c, CDiskFileWin32 PackageFile +0x60.
//   CInputEventPackage: Events +0x00, TimeStamp +0x0c, Checksum +0x10, MousePos +0x20, JoystickPos +0x28.
// Shape: an inline package constructor taking the time stamp by value (loaded before the vector copy) and the rest by
//   reference; the joystick through vector operator[] and CCountedPointer operator-> each time; the package's inline
//   STLport vector destructor (_Destroy(first, last) by value + free) at the end of the block.
extern "C" void __cdecl free(void* block);              // 0x00BFEA14

struct CAFile { virtual void v00(); };
struct C2DVector {
    float X, Y;
    C2DVector(float x, float y) : X(x), Y(y) {}
};
struct CChecksum { long Checksum1, Checksum2, Checksum3, Checksum4; };

struct CInputEvent {
    unsigned char _body[0x34];
    ~CInputEvent();                                     // 0x00A03B30
};

template <class T> inline void _Destroy(T* first, T* last)
{
    for (; first != last; ++first)
        first->~T();
}
template <class T>
struct vector {
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    vector(const vector& other);                        // 0x009F66A0 for CInputEvent
    ~vector()
    {
        _Destroy(_M_start, _M_finish);
        if (_M_start)
            free(_M_start);
    }
};

struct CMouse {
    virtual void v00();
    virtual void GetPosition(C2DVector& position);      // slot 1 (+0x04)
};
struct CJoystick {
    virtual void v00(); virtual void v01(); virtual void v02();
    virtual float GetAxisX();                           // slot 3 (+0x0c)
    virtual float GetAxisY();                           // slot 4 (+0x10)
};
struct CCountedJoystick {                               // CCountedPointer<CJoystick>
    CJoystick* P;
    void*      Count;
    CJoystick* operator->() const { return P; }
    CJoystick* Get() const { return P; }
};
struct CJoystickVector {
    CCountedJoystick* _M_start;
    CCountedJoystick* _M_finish;
    CCountedJoystick* _M_end_of_storage;
    CCountedJoystick& operator[](unsigned int n) { return *(_M_start + n); }
};

struct CInputEventPackage {
    vector<CInputEvent> Events;                         // +0x00
    long                TimeStamp;                      // +0x0c
    CChecksum           Checksum;                       // +0x10
    C2DVector           MousePos;                       // +0x20
    C2DVector           JoystickPos;                    // +0x28
    CInputEventPackage(const vector<CInputEvent>& events, long timeStamp, const CChecksum& checksum,
                       const C2DVector& mousePos, const C2DVector& joystickPos)
        : Events(events), TimeStamp(timeStamp), Checksum(checksum), MousePos(mousePos), JoystickPos(joystickPos) {}
    void Save(CAFile& file);                            // 0x009F52A0
};

struct CInputManager {
    void*               __vftable;                      // +0x00
    vector<CInputEvent>* _pad04[3];                     // +0x04 ProcessedEvents
    vector<CInputEvent> EventStore;                     // +0x10
    long                NoStoredEvents;                 // +0x1c
    long                NoEventScansRunning;            // +0x20
    CMouse*             PMouse;                         // +0x24
    void*               PKeyboard;                      // +0x28
    CJoystickVector     PJoysticks;                     // +0x2c
    bool                Loading;                        // +0x38
    bool                Saving;                         // +0x39
    CChecksum           Checksum;                       // +0x3c
    unsigned char       _pad4c[0x5c - 0x4c];            // +0x4c LoadedEventPackages, CurrentFilePackageIndex
    long                TimeStamp;                      // +0x5c
    CAFile              PackageFile;                    // +0x60 CDiskFileWin32
    long CalculateInternalChecksum();                   // 0x009F5170
    void SaveEventPackage();
};

void CInputManager::SaveEventPackage()
{
    if (NoStoredEvents == 0)
        return;
    C2DVector mousePos(0.0f, 0.0f);
    C2DVector joystickPos(0.0f, 0.0f);
    if (PMouse)
        PMouse->GetPosition(mousePos);
    if (PJoysticks[0].Get()) {
        joystickPos.X = PJoysticks[0]->GetAxisX();
        joystickPos.Y = PJoysticks[0]->GetAxisY();
    }
    Checksum.Checksum4 = CalculateInternalChecksum();
    CInputEventPackage package(EventStore, TimeStamp, Checksum, mousePos, joystickPos);
    package.Save(PackageFile);
}
