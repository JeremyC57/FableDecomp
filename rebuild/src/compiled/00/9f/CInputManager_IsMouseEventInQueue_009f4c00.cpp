// CInputManager::IsMouseEventInQueue(EInputEventType type, CInputEvent& event) -- copy out the first stored event of this type; false when there is none
// Leaf: ?IsMouseEventInQueue@CInputManager@@QBE_NW4EInputEventType@@AAVCInputEvent@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x009F4C00, 84 bytes. Name: Ego_r/FableWin PDB
//   ?IsMouseEventInQueue@CInputManager@@QBE_NW4EInputEventType@@AAVCInputEvent@@@Z (the only two-argument
//   (type, event&) query; ret 8), next to the landed IsJoystickEventInQueue (0x009F4C60) as in the Ego_r donor order.
// Callee: CInputEvent::GetType() const @ 0x00A03B40 (`mov eax,[ecx+28h] ; ret`).
// Shape: STLport `*(begin() + i)` re-reads EventStore._M_start every pass (`lea ecx,[ebx+eax]`, ebx = i*0x34);
//   the found event is copied with the implicit CInputEvent assignment (13 dwords, `rep movsd`).
// Layout: retail CInputManager (12-byte STLport vectors): vptr +0, ProcessedEvents +0x04, EventStore +0x10,
//   NoStoredEvents +0x1c (no engine header). CInputEvent 0x34 bytes (PDB): Data +0, DeviceType +0x20,
//   DeviceNumber +0x24, Type +0x28, CurrentTime +0x2c, StartTime +0x30.
enum EInputEventType { EIET_NONE };

class CInputEvent {
public:
    unsigned char Data[0x20];        // +0x00 UInputEventData
    long          DeviceType;        // +0x20
    long          DeviceNumber;      // +0x24
    EInputEventType Type;            // +0x28
    float         CurrentTime;       // +0x2c
    float         StartTime;         // +0x30
    EInputEventType GetType() const; // 0x00A03B40
};

namespace std {
template <class _Tp> class allocator { };
template <class _Value, class _Tp, class _MaybeReboundAlloc> struct _STLP_alloc_proxy { _Value _M_data; };
template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;
};
template <class _Tp, class _Alloc = allocator<_Tp> > class vector : protected _Vector_base<_Tp, _Alloc> {
public:
    typedef _Tp value_type;
    typedef const _Tp* const_iterator;
    typedef const _Tp& const_reference;
    typedef unsigned int size_type;
    const_iterator begin() const { return this->_M_start; }
    const_reference operator[](size_type __n) const { return *(begin() + __n); }
};
}   // namespace std

class CInputManager {
public:
    virtual ~CInputManager();
    std::vector<CInputEvent> ProcessedEvents;   // +0x04
    std::vector<CInputEvent> EventStore;        // +0x10
    long NoStoredEvents;                        // +0x1c
    bool IsMouseEventInQueue(EInputEventType type, CInputEvent& event) const;
};

bool CInputManager::IsMouseEventInQueue(EInputEventType type, CInputEvent& event) const
{
    for (long i = 0; i < NoStoredEvents; ++i) {
        if (EventStore[i].GetType() == type) {
            event = EventStore[i];
            return true;
        }
    }
    return false;
}