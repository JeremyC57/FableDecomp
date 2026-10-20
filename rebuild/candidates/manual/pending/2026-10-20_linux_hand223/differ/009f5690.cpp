// Grade: DIFFER(264v262) -- push_back's capacity test loads end_of_storage into eax (`mov eax,[esi+8]; cmp ecx,eax`, retail `cmp ecx,[esi+8]`) and the edi push/load is scheduled two stores earlier
// CInputEventPackage::CInputEventPackage(CAFile& file) -- load a recorded input package: event count, time stamp, checksum, mouse and joystick positions, then each 0x34-byte event, restamped against the current time, appended to Events
// Leaf: ??0CInputEventPackage
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009F5690, length 262 bytes (+ int3 padding), `ret 4`.
// Identity: the catalog/manifest label "Save" is wrong -- the body fills `this` from the file and returns it; the
//   Ego_d/FableWin PDB name for this body is ??0CInputEventPackage@@QAE@AAVCAFile@@@Z (0x00B491C0 there; Save is the
//   neighbour ?Save@CInputEventPackage@@QAEXAAVCAFile@@@Z).
// Callees: CAFile virtual slot 3 (+0x0c) Read(void*, long, long), CInputEvent() @ 0x00A04410 (catalog label
//   "CDefaultEngineMemoryInit" is propagated), ~CInputEvent @ 0x00A03B30, CInputEvent(const CInputEvent&) @ 0x009E4850,
//   GFGetTime() @ 0x009E1BC0 (double), STLport vector<CInputEvent>::_M_insert_overflow @ 0x009F6AC0.
// Layout (retail, STLport vector of 12 bytes -- the Ego_r PDB's Dinkumware vector is 16): Events +0x00 (start, finish,
//   end of storage), TimeStamp +0x0c, CChecksum Checksum +0x10 (four longs, zeroed by its inline ctor), C2DVector
//   MousePos +0x20, JoystickPos +0x28. CInputEvent (PDB, 0x34 bytes): CurrentTime +0x2c, StartTime +0x30.
// Shape: STLport push_back (finish != end_of_storage -> placement copy with the null test, else _M_insert_overflow with
//   a __false_type temporary, fill 1, at end).
inline void* operator new(unsigned int, void* where) { return where; }

struct CAFile {
    virtual void v00(); virtual void v01(); virtual void v02();
    virtual long Read(void* buffer, long size, long flags = 0);   // slot 3 (+0x0c)
};

double GFGetTime();                                     // 0x009E1BC0

struct CInputEvent {
    unsigned char Data[0x20];                           // +0x00 UInputEventData
    long  DeviceType;                                   // +0x20
    long  DeviceNumber;                                 // +0x24
    long  Type;                                         // +0x28
    float CurrentTime;                                  // +0x2c
    float StartTime;                                    // +0x30
    CInputEvent();                                      // 0x00A04410
    CInputEvent(const CInputEvent& other);              // 0x009E4850
    ~CInputEvent();                                     // 0x00A03B30
};

template <class T1, class T2> inline void _Copy_Construct(T1* p, const T2& val) { new (p) T1(val); }
struct __false_type { __false_type() {} };
template <class T> struct _STLP_alloc_proxy { T _M_data; };
template <class T>
struct vector {
    T* _M_start;
    T* _M_finish;
    _STLP_alloc_proxy<T*> _M_end_of_storage;
    vector() : _M_start(0), _M_finish(0) { _M_end_of_storage._M_data = 0; }
    void _M_insert_overflow(T* pos, const T& x, const __false_type&, unsigned int fillLen, bool atEnd);   // 0x009F6AC0
    void push_back(const T& x)
    {
        if (_M_finish != _M_end_of_storage._M_data) {
            _Copy_Construct(_M_finish, x);
            ++_M_finish;
        } else
            _M_insert_overflow(_M_finish, x, __false_type(), 1, true);
    }
};

struct CChecksum {
    long Checksum1, Checksum2, Checksum3, Checksum4;
    CChecksum() : Checksum1(0), Checksum2(0), Checksum3(0), Checksum4(0) {}
};
struct C2DVector { float X, Y; };

struct CInputEventPackage {
    vector<CInputEvent> Events;                         // +0x00
    long                TimeStamp;                      // +0x0c
    CChecksum           Checksum;                       // +0x10
    C2DVector           MousePos;                       // +0x20
    C2DVector           JoystickPos;                    // +0x28
    CInputEventPackage(CAFile& file);
};

CInputEventPackage::CInputEventPackage(CAFile& file)
{
    long count;
    file.Read(&count, 4);
    file.Read(&TimeStamp, 4);
    file.Read(&Checksum, 16);
    file.Read(&MousePos, 8);
    file.Read(&JoystickPos, 8);
    for (long i = 0; i < count; ++i) {
        CInputEvent event;
        file.Read(&event, sizeof(CInputEvent));
        event.CurrentTime = (float)GFGetTime();
        double age = event.StartTime;
        event.StartTime = (float)(GFGetTime() - age);
        Events.push_back(event);
    }
}
