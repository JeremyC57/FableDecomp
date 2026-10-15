#include <cstdio>
#include <cstring>
#include <cstdlib>

struct CInputEvent { unsigned char body[0x34]; };
struct CScan { bool Started; long Index; };
struct CMouse { virtual ~CMouse() { } CInputEvent EventStore[256]; long NoStoredEvents; long NoEventScansRunning;
    bool EventScanStart(CScan& scan, CInputEvent& ev) { scan.Started = true; ++NoEventScansRunning; if (NoStoredEvents > 0) { ++NoEventScansRunning; scan.Index = 0; ev = EventStore[0]; return true; } return false; } };
int main()
{
    CMouse* m = new CMouse; m->NoStoredEvents = 0; m->NoEventScansRunning = 0; for (int i = 0; i < 0x34; ++i) m->EventStore[0].body[i] = (unsigned char)(i * 3);
    CScan s; s.Started = false; s.Index = 9; CInputEvent ev; std::memset(&ev, 0, sizeof ev);
    if (m->EventScanStart(s, ev) || !s.Started || s.Index != 9 || m->NoEventScansRunning != 1) { std::printf("00a66b70_TEST FAIL\n"); return 1; }
    m->NoStoredEvents = 2; s.Started = false;
    if (!m->EventScanStart(s, ev) || !s.Started || s.Index != 0 || m->NoEventScansRunning != 3 || std::memcmp(&ev, &m->EventStore[0], 0x34)) { std::printf("00a66b70_TEST FAIL\n"); return 1; }
    delete m;
    std::printf("00a66b70_TEST PASS\n");
    return 0;
}