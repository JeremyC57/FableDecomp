// Behaviour test for 005c150b CIsSameItemType::operator() (standalone copy of the reconstructed body).
#include <stdio.h>
struct CItem { long ItemType; long NumberOfItems; };
struct CItem_Methods : CItem {
    bool IsSameItemTypeAsDefIndex(long defIndex);
    bool IsSlotFull();
};
static int g_slotFullCalls = 0;
static bool g_slotFull = false;
bool CItem_Methods::IsSameItemTypeAsDefIndex(long defIndex)
{
    if (this->NumberOfItems == 0)
        return false;
    return defIndex == this->ItemType;
}
bool CItem_Methods::IsSlotFull() { ++g_slotFullCalls; return g_slotFull; }
struct CIsSameItemType {
    long DefIndex;
    bool CheckSlotFull;
    bool operator()(CItem* item) const;
};
bool CIsSameItemType::operator()(CItem* item) const
{
    CItem_Methods* it = static_cast<CItem_Methods*>(item);
    return it->IsSameItemTypeAsDefIndex(DefIndex) && (!CheckSlotFull || !it->IsSlotFull());
}
int main()
{
    CItem item; item.ItemType = 7; item.NumberOfItems = 2;
    CIsSameItemType same; same.DefIndex = 7; same.CheckSlotFull = false;
    int ok = 1;
    if (!same(&item)) ok = 0;
    if (g_slotFullCalls != 0) ok = 0;
    same.CheckSlotFull = true; g_slotFull = false;
    if (!same(&item)) ok = 0;
    g_slotFull = true;
    if (same(&item)) ok = 0;
    if (g_slotFullCalls != 2) ok = 0;
    same.DefIndex = 8;
    if (same(&item)) ok = 0;
    item.NumberOfItems = 0; same.DefIndex = 7;
    if (same(&item)) ok = 0;
    if (g_slotFullCalls != 2) ok = 0;
    if (ok) printf("005c150b_TEST PASS\n");
    return ok ? 0 : 1;
}