// Behaviour test for 0x00687bd0 (CInputProcessGameBase::IsEventButtonHeld): the reconstructed predicate over a host input-event model.
#include <cstdio>
struct CInputEvent {
    long device; long kind; mutable int calls;
    CInputEvent(long d, long k) : device(d), kind(k), calls(0) {}
    long GetDeviceType() const { ++calls; return device; }
    long GetEventType() const { ++calls; return kind; }
};
struct CInputProcessGameBase {
    bool IsEventButtonHeld(const CInputEvent& event)
    {
        if (event.GetDeviceType() == 1)
            return event.GetEventType() == 0x15;
        if (event.GetDeviceType() == 2)
            return event.GetEventType() == 3;
        if (event.GetDeviceType() == 3)
            return event.GetEventType() == 6 || event.GetEventType() == 9 || event.GetEventType() == 12
                || (event.GetEventType() >= 32 && event.GetEventType() <= 36);
        return false;
    }
};
static bool Expect(long d, long k)
{
    if (d == 1) return k == 0x15;
    if (d == 2) return k == 3;
    if (d == 3) return k == 6 || k == 9 || k == 12 || (k >= 32 && k <= 36);
    return false;
}
int main()
{
    CInputProcessGameBase p;
    for (long d = 0; d <= 4; ++d)
        for (long k = -1; k <= 0x40; ++k) {
            CInputEvent e(d, k);
            if (p.IsEventButtonHeld(e) != Expect(d, k)) { std::printf("00687bd0_TEST FAIL\n"); return 1; }
        }
    CInputEvent j(3, 36);
    if (!p.IsEventButtonHeld(j) || j.calls != 8) { std::printf("00687bd0_TEST FAIL\n"); return 1; }
    std::printf("00687bd0_TEST PASS\n");
    return 0;
}