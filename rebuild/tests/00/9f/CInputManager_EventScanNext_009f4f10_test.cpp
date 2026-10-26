#include <cstdio>
#include <cstring>
class CInputEvent { public: long m_words[10]; long m_kind; long m_2c, m_30; long GetType() const { return m_kind; } };
class CInputEventScanInfo { public: bool Started; long Index; };
class CInputManager { public:
    virtual void v00() { }
    unsigned char _pad04[0x0c];
    CInputEvent* m_pEventStoreFirst; CInputEvent* m_pEventStoreLast; CInputEvent* m_pEventStoreEnd;
    long NoStoredEvents; long NoEventScansRunning;
    bool EventScanNext(CInputEventScanInfo& scan, CInputEvent& event)
    {
        for (long i = scan.Index + 1; i < NoStoredEvents; ++i) {
            CInputEvent* stored = &m_pEventStoreFirst[i];
            if (stored->GetType() != 0) { scan.Index = i; event = *stored; return true; }
        }
        return false;
    }
};
int main()
{
    CInputManager m; CInputEvent store[4]; std::memset(store, 0, sizeof store); store[0].m_kind = 1; store[2].m_kind = 7; store[2].m_words[3] = 33; store[3].m_kind = 2;
    m.m_pEventStoreFirst = store; m.NoStoredEvents = 4; CInputEventScanInfo scan; scan.Started = true; scan.Index = 0; CInputEvent out; std::memset(&out, 0, sizeof out);
    if (!m.EventScanNext(scan, out) || scan.Index != 2 || out.m_kind != 7 || out.m_words[3] != 33) { std::printf("009f4f10_TEST FAIL\n"); return 1; }
    if (!m.EventScanNext(scan, out) || scan.Index != 3 || out.m_kind != 2) { std::printf("009f4f10_TEST FAIL\n"); return 1; }
    if (m.EventScanNext(scan, out) || scan.Index != 3) { std::printf("009f4f10_TEST FAIL\n"); return 1; }
    scan.Index = -1; m.NoStoredEvents = 0; if (m.EventScanNext(scan, out)) { std::printf("009f4f10_TEST FAIL\n"); return 1; }
    std::printf("009f4f10_TEST PASS\n");
    return 0;
}