// CInputManager::SetEventsFromEventPackage(const CInputEventPackage& package) -- replay a recorded event package: clear the processed events, take the package's events as the event store (and their count), and hand the recorded mouse and joystick positions to the mouse and the first joystick
// Leaf: SetEventsFromEventPackage
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009F54D0, length 103 bytes.
// Callees: vector<CInputEvent>::erase(first, last) @ 0x009F67E0, vector<CInputEvent>::operator= @ 0x009F6890 (manifest
// "CCombatWheel::operator=" propagated), CMouse virtual slot 4 (+0x10) and CJoystick virtual slot 7 (+0x1c), each taking
// a C2DVector by address.
// Layout: retail CInputEventPackage (PDB with the 12-byte vector): Events +0x00, TimeStamp +0x0c, Checksum +0x10,
// MousePos +0x20, JoystickPos +0x28.
// Shape: the count is the pointer difference over the 0x34-byte element (`imul 4EC4EC4Fh ; sar edx,4`).
// Layout: retail CInputManager (0x80 bytes, 12-byte vectors; GOTCHAS wave 24m): ProcessedEvents +0x04, EventStore +0x10,
// NoStoredEvents +0x1c, NoEventScansRunning +0x20, PMouse +0x24, PKeyboard +0x28, PJoysticks +0x2c (8-byte
// CCountedPointer<CJoystick> elements), Loading +0x38, Saving +0x39, Checksum +0x3c, LoadedEventPackages +0x4c.
class CInputEvent {                                // 0x34 bytes (PDB): Data +0, DeviceType +0x20, DeviceNumber +0x24, Type +0x28
public:
    unsigned char _data[0x20];
    long          DeviceType;                      // +0x20
    long          DeviceNumber;                    // +0x24
    long          Type;                            // +0x28
    float         CurrentTime;                     // +0x2c
    float         StartTime;                       // +0x30
    long GetType() const;                          // 0x00A03B40
    long GetDeviceType() const;                    // 0x00A03B50
    long GetDeviceNumber() const;                  // 0x00A03B60
    long GetJoystickButton() const;                // 0x00A03B90 (movzx of the byte at +8)
};
template <class T> struct CRetailVector {          // STLport vector: first / last / end
    T* m_first; T* m_last; T* m_end;
    T* begin() { return m_first; }
    T* end() { return m_last; }
    T& operator[](long n) { return *(begin() + n); }
    long size() const { return m_last - m_first; }
    T* erase(T* first, T* last);                   // 0x009F67E0 (vector<CInputEvent>)
    void clear() { erase(begin(), end()); }
    CRetailVector& operator=(const CRetailVector& other);   // 0x009F6890
};
struct C2DVector { float x, y; };
class CJoystick {
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05(); virtual void v06();
    virtual void SetRecordedPosition(const C2DVector& position);   // slot 7 (+0x1c)
};
template <class T> struct CCountedPointer {        // {object, count block}
    T*    Data;
    void* m_block;
};
class CMouse {
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03();
    virtual void SetRecordedPosition(const C2DVector& position);   // slot 4 (+0x10)
};
class CKeyboard;
struct CInputEventPackage {
    CRetailVector<CInputEvent> Events;             // +0x00
    long                       TimeStamp;          // +0x0c
    unsigned char              Checksum[0x10];     // +0x10
    C2DVector                  MousePos;           // +0x20
    C2DVector                  JoystickPos;        // +0x28
};
class CInputManager {
public:
    virtual void v00();
    CRetailVector<CInputEvent> ProcessedEvents;    // +0x04
    CRetailVector<CInputEvent> EventStore;         // +0x10
    long                       NoStoredEvents;     // +0x1c
    long                       NoEventScansRunning;   // +0x20
    CMouse*                    PMouse;             // +0x24 scoped_ptr<CMouse>
    CKeyboard*                 PKeyboard;          // +0x28 scoped_ptr<CKeyboard>
    CRetailVector<CCountedPointer<CJoystick> > PJoysticks;   // +0x2c
    void SetEventsFromEventPackage(const CInputEventPackage& package);
};

void CInputManager::SetEventsFromEventPackage(const CInputEventPackage& package)
{
    ProcessedEvents.clear();
    EventStore = package.Events;
    NoStoredEvents = EventStore.size();
    if (PMouse)
        PMouse->SetRecordedPosition(package.MousePos);
    if (PJoysticks[0].Data)
        PJoysticks[0].Data->SetRecordedPosition(package.JoystickPos);
}