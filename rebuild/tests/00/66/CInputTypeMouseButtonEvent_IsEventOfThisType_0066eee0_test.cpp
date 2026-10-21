#include <cstdio>
class CInputEvent {
public:
    long GetType() const;                                // 0x00A03B40
    long GetDeviceType() const;                          // 0x00A03B50
};
class CInputTypeMouseButtonEvent {
public:
    virtual void v000();
    long Button;                                         // +0x04
    bool IsEventOfThisType(const CInputEvent& event) const;
};
bool CInputTypeMouseButtonEvent::IsEventOfThisType(const CInputEvent& event) const
{
    if (event.GetDeviceType() == 3) {
        if (event.GetType() == 4 || event.GetType() == 5 || event.GetType() == 6)
            return Button == 1;
        if (event.GetType() == 7 || event.GetType() == 8 || event.GetType() == 9)
            return Button == 3;
        if (event.GetType() == 10 || event.GetType() == 11 || event.GetType() == 12)
            return Button == 2;
        if (event.GetType() >= 22 && event.GetType() <= 26)
            return Button == event.GetType() - 14;
        if (event.GetType() >= 27 && event.GetType() <= 31)
            return Button == event.GetType() - 19;
        if (event.GetType() >= 32 && event.GetType() <= 36)
            return Button == event.GetType() - 24;
    }
    return false;
}

static int g_fail = 0;
static void check(bool c, const char* w) { if (!c) { std::printf("FAIL %s\n", w); g_fail = 1; } }
static long g_type, g_device;
long CInputEvent::GetType() const { return g_type; }
long CInputEvent::GetDeviceType() const { return g_device; }
void CInputTypeMouseButtonEvent::v000() {}
static bool Expected(long device, long type, long button)
{
    if (device != 3) return false;
    if (type >= 4 && type <= 6) return button == 1;
    if (type >= 7 && type <= 9) return button == 3;
    if (type >= 10 && type <= 12) return button == 2;
    if (type >= 22 && type <= 26) return button == type - 14;
    if (type >= 27 && type <= 31) return button == type - 19;
    if (type >= 32 && type <= 36) return button == type - 24;
    return false;
}
int main()
{
    CInputTypeMouseButtonEvent binding; CInputEvent event;
    int trues = 0;
    for (long device = 0; device <= 4; ++device)
        for (long type = -1; type <= 40; ++type)
            for (long button = 0; button <= 13; ++button) {
                g_device = device; g_type = type; binding.Button = button;
                bool got = binding.IsEventOfThisType(event);
                if (got) ++trues;
                if (got != Expected(device, type, button)) { std::printf("device %ld type %ld button %ld\n", device, type, button); check(false, "table"); }
            }
    check(trues == 9 + 15, "24 matching pairs");
    if (!g_fail) std::printf("0066eee0_TEST PASS\n");
    return g_fail;
}