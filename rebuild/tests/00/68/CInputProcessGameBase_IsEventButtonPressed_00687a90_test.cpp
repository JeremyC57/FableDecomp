// Behaviour test for 0x00687a90 (CInputProcessGameBase::IsEventButtonPressed): the reconstructed predicate over a host input-event model.
#include <cstdio>
struct CInputEvent {
    long device; long kind; mutable int calls;
    CInputEvent(long d, long k) : device(d), kind(k), calls(0) {}
    long GetDeviceType() const { ++calls; return device; }
    long GetEventType() const { ++calls; return kind; }
};
struct CInputProcessGameBase {
    bool IsEventButtonPressed(const CInputEvent& event)
    {
        if (event.GetDeviceType() == 1)
            return event.GetEventType() == 0x13;
        if (event.GetDeviceType() == 2)
            return event.GetEventType() == 1;
        if (event.GetDeviceType() == 3)
            return event.GetEventType() == 4 || event.GetEventType() == 7 || event.GetEventType() == 10
                || (event.GetEventType() >= 22 && event.GetEventType() <= 26);
        return false;
    }
};
static bool Expect(long d, long k)
{
    if (d == 1) return k == 0x13;
    if (d == 2) return k == 1;
    if (d == 3) return k == 4 || k == 7 || k == 10 || (k >= 22 && k <= 26);
    return false;
}
int main()
{
    CInputProcessGameBase p;
    for (long d = 0; d <= 4; ++d)
        for (long k = -1; k <= 0x40; ++k) {
            CInputEvent e(d, k);
            if (p.IsEventButtonPressed(e) != Expect(d, k)) { std::printf("00687a90_TEST FAIL\n"); return 1; }
        }
    CInputEvent j(3, 26);
    if (!p.IsEventButtonPressed(j) || j.calls != 8) { std::printf("00687a90_TEST FAIL\n"); return 1; }
    std::printf("00687a90_TEST PASS\n");
    return 0;
}