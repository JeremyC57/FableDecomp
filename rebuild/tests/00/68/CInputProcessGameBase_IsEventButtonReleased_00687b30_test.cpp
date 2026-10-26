// Behaviour test for 0x00687b30 (CInputProcessGameBase::IsEventButtonReleased): the reconstructed predicate over a host input-event model.
#include <cstdio>
class CInputEvent { public:
    long device; long kind; mutable int calls;
    CInputEvent(long d, long k) : device(d), kind(k), calls(0) {}
    long GetDeviceType() const { ++calls; return device; }
    long GetEventType() const { ++calls; return kind; }
};
struct CInputProcessGameBase {
    bool IsEventButtonReleased(const CInputEvent& event)
    {
        if (event.GetDeviceType() == 1)
            return event.GetEventType() == 0x14;
        if (event.GetDeviceType() == 2)
            return event.GetEventType() == 2;
        if (event.GetDeviceType() == 3)
            return event.GetEventType() == 5 || event.GetEventType() == 8 || event.GetEventType() == 11
                || (event.GetEventType() >= 27 && event.GetEventType() <= 31);
        return false;
    }
};
static bool Expect(long d, long k)
{
    if (d == 1) return k == 0x14;
    if (d == 2) return k == 2;
    if (d == 3) return k == 5 || k == 8 || k == 11 || (k >= 27 && k <= 31);
    return false;
}
int main()
{
    CInputProcessGameBase p;
    for (long d = 0; d <= 4; ++d)
        for (long k = -1; k <= 0x40; ++k) {
            CInputEvent e(d, k);
            if (p.IsEventButtonReleased(e) != Expect(d, k)) { std::printf("00687b30_TEST FAIL\n"); return 1; }
        }
    CInputEvent j(3, 31);
    if (!p.IsEventButtonReleased(j) || j.calls != 8) { std::printf("00687b30_TEST FAIL\n"); return 1; }
    std::printf("00687b30_TEST PASS\n");
    return 0;
}