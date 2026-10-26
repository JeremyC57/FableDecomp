#include <cstdio>
class CSystemManager { public:
    bool ApplicationActive, EverBeenActive, Restore;
    void SetAsActive(bool active)
    {
        if (active && !ApplicationActive) {
            if (EverBeenActive == true)
                Restore = true;
        }
        ApplicationActive = active;
        EverBeenActive = true;
    }
};
int main()
{
    CSystemManager s; s.ApplicationActive = false; s.EverBeenActive = false; s.Restore = false;
    s.SetAsActive(true);   // first activation: no restore
    if (!s.ApplicationActive || !s.EverBeenActive || s.Restore) { std::printf("009a5270_TEST FAIL\n"); return 1; }
    s.SetAsActive(false);
    if (s.ApplicationActive || !s.EverBeenActive || s.Restore) { std::printf("009a5270_TEST FAIL\n"); return 1; }
    s.SetAsActive(true);   // re-activation: restore
    if (!s.ApplicationActive || !s.Restore) { std::printf("009a5270_TEST FAIL\n"); return 1; }
    s.Restore = false; s.SetAsActive(true);   // already active: nothing new
    if (!s.ApplicationActive || s.Restore) { std::printf("009a5270_TEST FAIL\n"); return 1; }
    std::printf("009a5270_TEST PASS\n");
    return 0;
}