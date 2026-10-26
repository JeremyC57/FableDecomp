#include <cstdio>
#include <cstring>
class CInputEvent { public: unsigned char _body[0x34]; };
template<class T> struct CRetailVector { T* first; T* last; T* end_of_storage; struct iterator { T* p; explicit iterator(T* q) : p(q) {} T& operator*() const { return *p; } }; iterator begin() { return iterator(first); } };
class CInputEventScanInfo { public: bool Started; long Index; };
class CInputManager { public:
    virtual void v00() { }
    unsigned char _pad04[0x0c];
    CRetailVector<CInputEvent> EventStore;
    long NoStoredEvents; long NoEventScansRunning;
    bool EventScanStart(CInputEventScanInfo& scan, CInputEvent& event)
    {
        scan.Started = true;
        ++NoEventScansRunning;
        if (NoStoredEvents > 0) { scan.Index = 0; event = *EventStore.begin(); return true; }
        return false;
    }
};
int main()
{
    CInputManager m; CInputEvent store[2]; std::memset(store, 0x11, sizeof store); store[0]._body[5] = 42; m.EventStore.first = store; m.NoStoredEvents = 2; m.NoEventScansRunning = 3;
    CInputEventScanInfo scan; scan.Started = false; scan.Index = 9; CInputEvent out; std::memset(&out, 0, sizeof out);
    if (!m.EventScanStart(scan, out) || !scan.Started || scan.Index != 0 || m.NoEventScansRunning != 4 || out._body[5] != 42 || std::memcmp(&out, &store[0], sizeof out)) { std::printf("009f4ed0_TEST FAIL\n"); return 1; }
    m.NoStoredEvents = 0; scan.Started = false; scan.Index = 9; if (m.EventScanStart(scan, out) || !scan.Started || scan.Index != 9 || m.NoEventScansRunning != 5) { std::printf("009f4ed0_TEST FAIL\n"); return 1; }
    std::printf("009f4ed0_TEST PASS\n");
    return 0;
}