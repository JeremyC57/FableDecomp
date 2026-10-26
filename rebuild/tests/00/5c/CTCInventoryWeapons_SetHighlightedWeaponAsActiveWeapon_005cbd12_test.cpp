#include <cstdio>
// Behaviour of CTCInventoryWeapons::SetHighlightedWeaponAsActiveWeapon (0x005CBD12).
struct CComponent;
struct CTCInventoryBase_Retail { long m_highlighted; long GetHighlightedItem() { return m_highlighted; } };
class CTCInventoryWeapons : public CTCInventoryBase_Retail { public:
    long m_active; long m_calls;
    void SetThingAsActiveWeapon(long defIndex, CComponent* menuEntry) { ++m_calls; if (!menuEntry) m_active = defIndex; }
    void SetHighlightedWeaponAsActiveWeapon()
    {
        long item = GetHighlightedItem();
        if (item > 0)
            SetThingAsActiveWeapon(item, 0);
    }
};
int main()
{
    CTCInventoryWeapons w; w.m_active = 0; w.m_calls = 0; w.m_highlighted = 0;
    w.SetHighlightedWeaponAsActiveWeapon();
    int bad = (w.m_calls != 0);
    w.m_highlighted = -3; w.SetHighlightedWeaponAsActiveWeapon(); bad |= (w.m_calls != 0);
    w.m_highlighted = 44; w.SetHighlightedWeaponAsActiveWeapon(); bad |= (w.m_calls != 1 || w.m_active != 44);
    if (bad) { std::printf("005cbd12_TEST FAIL\n"); return 1; }
    std::printf("005cbd12_TEST PASS\n");
    return 0;
}