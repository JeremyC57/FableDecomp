// CInputManager::AddEventToStore(const CInputEvent& event) -- append the event to the EventStore vector (copy-constructed in place when there is room, else through the vector's reallocating insert), count it in NoStoredEvents and return true
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009F5600, length 80 bytes.
// Leaf: ?AddEventToStore@CInputManager@@IAE_NABVCInputEvent@@@Z
// PDB: ?AddEventToStore@CInputManager@@IAE_NABVCInputEvent@@@Z (protected, bool). The chunk label "CInputEventVector_PushBackAt0x14"
//   is a descriptive autoname of the inlined push_back.
// Callees: CInputEvent::CInputEvent(const CInputEvent&) @ 0x009E4850, vector<CInputEvent>::_M_insert_overflow @ 0x009F6AC0
//   (where, value, const __false_type&, count 1, at_end true) -- the same push_back as the landed 0x009F6D50.
// Layout: retail CInputManager (12-byte STLport vectors, see the landed ClearEvents 0x009F5540): EventStore +0x10, NoStoredEvents +0x1c.
// Shape: the inlined STLport push_back on the member (`lea esi,[edi+10h]` for the vector `this`, `add [esi+4],34h` for ++_M_finish);
//   the empty __false_type tag is homed in this function's dead argument slot (`lea edx,[esp+14h]`), so it needs a user constructor.
#include <new>

class CInputEvent {                                  // 0x34 bytes
public:
    unsigned char _data[0x34];
    CInputEvent(const CInputEvent& other);           // 0x009E4850
};

struct __false_type { __false_type() {} };

template <class T> class CRetailVector {             // STLport vector: _M_start / _M_finish / _M_end_of_storage
public:
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    void _M_insert_overflow(T* position, const T& x, const __false_type& tag, unsigned int fillLen, bool atEnd);   // 0x009F6AC0
    void push_back(const T& x)
    {
        if (_M_finish != _M_end_of_storage) {
            new (_M_finish) T(x);
            ++_M_finish;
        }
        else
            _M_insert_overflow(_M_finish, x, __false_type(), 1, true);
    }
};

class CInputManager {
public:
    virtual void v00();
    CRetailVector<CInputEvent> ProcessedEvents;      // +0x04
    CRetailVector<CInputEvent> EventStore;           // +0x10
    long                       NoStoredEvents;       // +0x1c
protected:
    bool AddEventToStore(const CInputEvent& event);
};

bool CInputManager::AddEventToStore(const CInputEvent& event)
{
    EventStore.push_back(event);
    ++NoStoredEvents;
    return true;
}