// Behaviour test for 0x00AB6E40 CJoystickDX::UpdateEvents: a standalone copy of the reconstructed control flow over a fake
// DirectInput device (Poll / Acquire / GetDeviceData with DIERR_INPUTLOST retries) and a fake event store.
#include <cstdio>
#include <vector>
static const long DIERR_INPUTLOST = (long)0x8007001E;
struct Data { unsigned int dwOfs, dwData, dwTimeStamp, dwSequence, uAppData; };
struct Event { int type; long button; float x, y; double time; };   // 17 position, 18 position 2, 19 pressed, 21 released
struct FakeDevice {
    std::vector<Data> queue; int pollLostOnce; int dataLostOnce; int acquires; long pollResult;
    long Poll() { if (pollLostOnce) { pollLostOnce = 0; return DIERR_INPUTLOST; } return pollResult; }
    long Acquire() { ++acquires; return 0; }
    long GetDeviceData(unsigned int size, Data* d, unsigned int* count, unsigned int) {
        if (size != 0x14) return -1;
        if (dataLostOnce) { dataLostOnce = 0; return DIERR_INPUTLOST; }
        if (queue.empty()) { *count = 0; return 0; }
        *d = queue.front(); queue.erase(queue.begin()); *count = 1; return 0;
    }
};
struct Joy {
    FakeDevice* dev; float XAxis, YAxis, XAxis2, YAxis2; std::vector<Event> store; int processed; int emptied;
    bool UpdateEvents() {
        unsigned int count = 1;
        Event ev = { 0, 0, 0, 0, 0 };
        if (dev) {
            store.clear(); ++emptied;
            long r = dev->Poll();
            if (r == DIERR_INPUTLOST) { dev->Acquire(); r = dev->Poll(); }
            if (r != 0 && r != 1) return false;
            Data d; count = 1;
            r = dev->GetDeviceData(sizeof(Data), &d, &count, 0);
            if (r == DIERR_INPUTLOST) { dev->Acquire(); r = dev->GetDeviceData(sizeof(Data), &d, &count, 0); }
            if (r != 0 && r != 1) return false;
            while (count > 0) {
                bool add = false;
                float v = ((float)(int)d.dwData * 2.0f) / 65536.0f - 1.0f;
                switch (d.dwOfs) {
                case 0x00: XAxis = v; break;
                case 0x04: YAxis = v; break;
                case 0x08: XAxis2 = v; break;
                case 0x0c: case 0x10: add = false; break;
                case 0x14: YAxis2 = v; break;
                default:
                    if (d.dwOfs >= 0x30 && d.dwOfs <= 0x3b) {
                        Event e = { (d.dwData & 0x80) ? 19 : 21, (long)(d.dwOfs - 0x30 + 1), XAxis, YAxis, (double)d.dwTimeStamp };
                        ev = e; add = true;
                    }
                }
                if (add) { store.push_back(ev); ++processed; }
                count = 1;
                r = dev->GetDeviceData(sizeof(Data), &d, &count, 0);
                if (r == DIERR_INPUTLOST) { dev->Acquire(); r = dev->GetDeviceData(sizeof(Data), &d, &count, 0); }
                if (r != 0 && r != 1) return false;
            }
            Event p1 = { 17, 0, XAxis, YAxis, 0.0 }; store.push_back(p1);
            Event p2 = { 18, 0, XAxis2, YAxis2, 0.0 }; store.push_back(p2);
        }
        return true;
    }
};
int main() {
    int fails = 0;
    FakeDevice dev = { std::vector<Data>(), 1, 1, 0, 0 };
    Data a = { 0x00, 65536, 1, 0, 0 }, b = { 0x04, 0, 2, 0, 0 }, c = { 0x31, 0x80, 0xFFFFFFF0u, 0, 0 }, e = { 0x3b, 0, 7, 0, 0 },
         f = { 0x0c, 123, 8, 0, 0 }, g = { 0x20, 5, 9, 0, 0 };
    dev.queue.push_back(a); dev.queue.push_back(b); dev.queue.push_back(c); dev.queue.push_back(e); dev.queue.push_back(f); dev.queue.push_back(g);
    Joy j = { &dev, 0, 0, 0, 0, std::vector<Event>(), 0, 0 };
    if (!j.UpdateEvents()) ++fails;
    if (dev.acquires != 2) ++fails;                                   // one retry for Poll, one for GetDeviceData
    if (j.XAxis != 1.0f || j.YAxis != -1.0f) ++fails;                  // 65536 -> +1, 0 -> -1
    if (j.store.size() != 4 || j.processed != 2) ++fails;              // two button events + two position events
    if (j.store.size() == 4) {
        if (j.store[0].type != 19 || j.store[0].button != 2 || j.store[0].time != 4294967280.0) ++fails;   // unsigned timestamp
        if (j.store[1].type != 21 || j.store[1].button != 12) ++fails;
        if (j.store[2].type != 17 || j.store[3].type != 18) ++fails;
    }
    FakeDevice bad = { std::vector<Data>(), 0, 0, 0, 5 };
    Joy k = { &bad, 0, 0, 0, 0, std::vector<Event>(), 0, 0 };
    if (k.UpdateEvents()) ++fails;                                     // Poll result 5 -> false
    Joy n = { 0, 0, 0, 0, 0, std::vector<Event>(), 0, 0 };
    if (!n.UpdateEvents() || n.emptied != 0) ++fails;                  // no device -> true, nothing touched
    if (fails) { std::printf("00ab6e40_TEST FAIL %d\n", fails); return 1; }
    std::printf("00ab6e40_TEST PASS\n");
    return 0;
}