// Behaviour test for 0x009444E0: warehouse-work filter predicate.
#include <cstdio>
struct CCrateThing { unsigned char m_bCarried; bool accepts; bool CanBePutIn(void* p) { return accepts && p != 0; } };
struct CTCVillage { bool task, cook; bool IsThereACratePutDownTask() { return task; } bool IsCookTime_00658b20() { return cook; } };
struct CIP { CCrateThing* p; CCrateThing* GetPItem() const { return p; } };
struct CFilter {
    CTCVillage* m_pVillage; CIP m_pCrate; CIP m_pWarehouse;
    bool Eval()
    {
        if (m_pCrate.GetPItem() != 0 && m_pWarehouse.GetPItem() != 0) {
            if (!m_pCrate.GetPItem()->m_bCarried || m_pVillage->IsThereACratePutDownTask()) {
                if (m_pCrate.GetPItem()->CanBePutIn(m_pWarehouse.GetPItem())) {
                    if (m_pVillage == 0 || m_pVillage->IsCookTime_00658b20())
                        return true;
                }
            }
        }
        return false;
    }
};
int main()
{
    CCrateThing crate = { 0, true }, house = { 0, true };
    CTCVillage v = { false, true };
    CFilter f; f.m_pVillage = &v; f.m_pCrate.p = &crate; f.m_pWarehouse.p = &house;
    int bad = 0;
    if (!f.Eval()) bad = 1;
    crate.m_bCarried = 1; if (f.Eval()) bad = 2;
    v.task = true; if (!f.Eval()) bad = 3;
    v.cook = false; if (f.Eval()) bad = 4;
    f.m_pVillage = 0; crate.m_bCarried = 0; if (!f.Eval()) bad = 5;
    crate.accepts = false; if (f.Eval()) bad = 6;
    crate.accepts = true; f.m_pWarehouse.p = 0; if (f.Eval()) bad = 7;
    if (bad) { std::printf("009444e0_TEST FAIL\n"); return 1; }
    std::printf("009444e0_TEST PASS\n");
    return 0;
}