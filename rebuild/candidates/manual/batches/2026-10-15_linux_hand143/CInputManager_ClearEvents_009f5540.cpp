// CInputManager::ClearEvents() -- empty the processed and stored event vectors, zero the stored-event count, and clear every joystick (its virtual reset plus Clear), the mouse and the keyboard
// Leaf: ClearEvents
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009F5540, length 123 bytes.
// Callees: vector<CInputEvent>::erase(first, last) @ 0x009F67E0 (x2), the joystick's virtual slot 15 (+0x3c) and Clear
// @ 0x009E42C0, CMouse::Clear @ 0x00A66C00, CKeyboard::Clear @ 0x00A66860 (tail jump; the three Clear labels read
// "CMemoryDataOutputStream::Clear", propagated).
// Shape: `clear()` is the inlined `erase(begin(), end())` (end loaded first); the joystick count is a `long` local
// (`sar edi,3`), each joystick re-indexed from the vector's first pointer for both calls.
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
};
class CJoystick {
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07();
    virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14();
    virtual void ResetState();                     // slot 15 (+0x3c)
    void Clear();                                  // 0x009E42C0
};
template <class T> struct CCountedPointer {        // {object, count block}
    T*    Data;
    void* m_block;
};
class CMouse {
public:
    void Clear();                                  // 0x00A66C00
};
class CKeyboard {
public:
    void Clear();                                  // 0x00A66860
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
    void ClearEvents();
};

void CInputManager::ClearEvents()
{
    ProcessedEvents.clear();
    EventStore.clear();
    NoStoredEvents = 0;
    long count = PJoysticks.size();
    for (long i = 0; i < count; ++i) {
        if (PJoysticks[i].Data) {
            PJoysticks[i].Data->ResetState();
            PJoysticks[i].Data->Clear();
        }
    }
    if (PMouse)
        PMouse->Clear();
    if (PKeyboard)
        PKeyboard->Clear();
}
