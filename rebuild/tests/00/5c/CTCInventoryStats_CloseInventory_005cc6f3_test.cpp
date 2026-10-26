#include <cstdio>
// Behaviour of CTCInventoryStats::CloseInventory (0x005CC6F3).
class CThing { public: long m_kills; bool m_lastArg; void Kill(bool immediate) { ++m_kills; m_lastArg = immediate; } };
struct CTCInventoryBase_Retail { bool InventoryOpen; };
class CTCInventoryStats : public CTCInventoryBase_Retail { public:
    CThing* pMannequin; bool UseMannequin;
    void CloseInventory()
    {
        UseMannequin = false;
        if (pMannequin) {
            pMannequin->Kill(false);
            pMannequin = 0;
        }
        InventoryOpen = false;
    }
};
int main()
{
    CThing mannequin; mannequin.m_kills = 0; mannequin.m_lastArg = true;
    CTCInventoryStats s; s.InventoryOpen = true; s.UseMannequin = true; s.pMannequin = &mannequin;
    s.CloseInventory();
    int bad = (mannequin.m_kills != 1 || mannequin.m_lastArg || s.pMannequin != 0 || s.UseMannequin || s.InventoryOpen);
    s.InventoryOpen = true; s.UseMannequin = true; s.CloseInventory();
    bad |= (mannequin.m_kills != 1 || s.UseMannequin || s.InventoryOpen);
    if (bad) { std::printf("005cc6f3_TEST FAIL\n"); return 1; }
    std::printf("005cc6f3_TEST PASS\n");
    return 0;
}