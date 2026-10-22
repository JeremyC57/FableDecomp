#include <cstdio>
// Behaviour of CTCInventoryWeapons::vfunc_33 (0x005CBCEA).
struct CComponent;
bool IsAugmentation(long defIndex) { return defIndex >= 100; }
struct CTCInventoryWeapons {
    long m_active; long m_calls;
    void SetThingAsActiveWeapon(long defIndex, CComponent* menuEntry) { ++m_calls; if (!menuEntry) m_active = defIndex; }
    void vfunc_33(long defIndex, bool* handled)
    {
        *handled = false;
        if (IsAugmentation(defIndex))
            SetThingAsActiveWeapon(defIndex, 0);
    }
};
int main()
{
    CTCInventoryWeapons w; w.m_active = 0; w.m_calls = 0;
    bool handled = true;
    w.vfunc_33(5, &handled);
    int bad = (handled || w.m_calls != 0);
    handled = true; w.vfunc_33(105, &handled);
    bad |= (handled || w.m_calls != 1 || w.m_active != 105);
    if (bad) { std::printf("005cbcea_TEST FAIL\n"); return 1; }
    std::printf("005cbcea_TEST PASS\n");
    return 0;
}