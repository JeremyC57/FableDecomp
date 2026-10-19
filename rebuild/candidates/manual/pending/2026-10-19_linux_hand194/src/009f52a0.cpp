// CInputEventPackage::Save(CDataOutputStream& stream) const -- write the event count, time stamp, checksum, mouse and joystick positions, then every event with its start time rebased to "now minus start" and a cleared current time
// Leaf: Save
// Retail: Fable.exe (TLC, VC7.1) @ 0x009F52A0, 234 bytes, `ret 4`.
// Callees: the stream's virtual slot 4 (+0x10, Write(data, size, flags)), CInputEvent::CInputEvent(const CInputEvent&) @
//   0x009E4850, GFGetTime @ 0x009E1BC0 (`__fastcall`, double), CInputEvent::~CInputEvent @ 0x00A03B30 (an empty `ret`,
//   labelled "$E2").
// Layout: Ego_r.pdb CInputEventPackage with the 12-byte retail vector: Events +0x00, TimeStamp +0x0c, Checksum (16 bytes)
//   +0x10, MousePos +0x20, JoystickPos +0x28; CInputEvent 0x34 bytes (CurrentTime +0x2c, StartTime +0x30).
// Shape: the count is `last - first` (/0x34 by the reciprocal multiply) in a frame local written first; each element is
//   copied into a frame event (copy ctor), the start time is read into a double local before the GFGetTime call (the
//   double makes VC align the frame: `and esp,-8`), the current time cleared, and the start time assigned
//   `(float)GFGetTime() - startTime` (that form sinks the `fstp` below the Write pushes; `(float)(now - start)` stores
//   before them); the element address is `first + offset` with the offset strength-reduced in the frame.
class CInputEvent {                                // 0x34 bytes
public:
    unsigned char _data[0x20];
    long          DeviceType;                      // +0x20
    long          DeviceNumber;                    // +0x24
    long          Type;                            // +0x28
    float         CurrentTime;                     // +0x2c
    float         StartTime;                       // +0x30
    CInputEvent(const CInputEvent& other);         // 0x009E4850
    ~CInputEvent();                                // 0x00A03B30
};
double __fastcall GFGetTime();                     // 0x009E1BC0
struct CChecksum { unsigned long Words[4]; };      // 16 bytes
struct C2DVector { float X, Y; };
class CDataOutputStream {
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03();
    virtual void Write(const void* data, long size, long flags);   // slot 4 (+0x10)
};
template <class T> struct CRetailVector {          // STLport vector: first / last / end
    T* m_first; T* m_last; T* m_end;
    long size() const { return m_last - m_first; }
    T& operator[](long n) const { return *(m_first + n); }
};
class CInputEventPackage {
public:
    CRetailVector<CInputEvent> Events;             // +0x00
    long                       TimeStamp;          // +0x0c
    CChecksum                  Checksum;           // +0x10
    C2DVector                  MousePos;           // +0x20
    C2DVector                  JoystickPos;        // +0x28
    void Save(CDataOutputStream& stream) const;
};

void CInputEventPackage::Save(CDataOutputStream& stream) const
{
    long count = Events.size();
    stream.Write(&count, 4, 0);
    stream.Write(&TimeStamp, 4, 0);
    stream.Write(&Checksum, 0x10, 0);
    stream.Write(&MousePos, 8, 0);
    stream.Write(&JoystickPos, 8, 0);
    for (long i = 0; i < count; ++i) {
        CInputEvent event(Events[i]);
        double startTime = event.StartTime;
        event.CurrentTime = 0.0f;
        event.StartTime = (float)GFGetTime() - startTime;
        stream.Write(&event, 0x34, 0);
    }
}
