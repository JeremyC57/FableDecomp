// Behaviour test for CAIStateGroup_OpenAndCloseShop::IsRunnable (0x00926C50): model copy, scripted stubs.
#include <cstdio>

struct C3DVector { float x, y, z; };
struct CTCShop {
    long StockState;
    bool openHours, permanent;
    bool IsOpeningHours() { return openHours; }
    bool IsPermanentlyStocked() { return permanent; }
};
struct CTCShopKeeper {
    CTCShop* shop;
    bool othersInShop;
    int otherCalls;
    CTCShop* GetShopTC() { return shop; }
    bool GetAreOtherPeopleInShop(bool flag) { ++otherCalls; return flag ? false : othersInShop; }
};
struct CTCShopSign { bool m_closed; };
struct CThingBuilding {
    unsigned char m_flags91;
    unsigned char m_buildingFlags;
    bool hasSign, occupants, busy, inside;
    CTCShopSign sign;
    bool GetInterface(CTCShopSign** out, long type) { if (type != 0xbd || !hasSign) return false; *out = &sign; return true; }
    bool HasOccupants() { return occupants; }
    bool HasBusyOccupants() { return busy; }
    bool IsPointInside(const C3DVector& p) { return inside && p.x == 1.0f; }
};
struct CThingShopkeeper {
    CThingBuilding* shop;
    CThingBuilding* home;
    CTCShopKeeper* keeper;
    C3DVector pos;
    CThingBuilding* GetPShop() const { return shop; }
    CThingBuilding* GetPHome() const { return home; }
    bool GetInterface(CTCShopKeeper** out, long type) { if (type != 0x90 || !keeper) return false; *out = keeper; return true; }
    const C3DVector& GetPosition() const { return pos; }
};
struct CAIBrain { CThingShopkeeper* PCreature; };
struct CAIStateGroup_OpenAndCloseShop {
    CAIBrain* PBrain;
    bool GoToInsideFace;
    bool Lock;
    CThingShopkeeper* GetCreature() const { return PBrain->PCreature; }
    bool IsRunnable();
};

bool CAIStateGroup_OpenAndCloseShop::IsRunnable()
{
    CThingBuilding* shop = GetCreature()->GetPShop();
    if (shop && !(shop->m_flags91 & 1)) {
        CTCShopKeeper* keeper;
        if (GetCreature()->GetInterface(&keeper, 0x90) && keeper->GetShopTC()) {
            shop->m_buildingFlags |= 1;
            bool open = keeper->GetShopTC() && keeper->GetShopTC()->IsOpeningHours();
            CTCShopSign* sign;
            if (!shop->HasOccupants()) {
                if (shop->GetInterface(&sign, 0xbd))
                    sign->m_closed = !open;
                return false;
            }
            if (!open && shop->GetInterface(&sign, 0xbd))
                sign->m_closed = true;
            if (shop->HasBusyOccupants()) {
                if (!open) {
                    CThingShopkeeper* creature = GetCreature();
                    CThingBuilding* myShop = creature->GetPShop();
                    CThingBuilding* home = GetCreature()->GetPHome();
                    if (shop->IsPointInside(creature->GetPosition()) != (myShop == home)) {
                        GoToInsideFace = shop->IsPointInside(GetCreature()->GetPosition());
                        Lock = false;
                        return true;
                    }
                    if (!keeper->GetAreOtherPeopleInShop(false))
                        return false;
                }
                GoToInsideFace = shop->IsPointInside(GetCreature()->GetPosition());
                Lock = false;
                return true;
            }
            CTCShop* shopTC = keeper->GetShopTC();
            if (shopTC && (shopTC->IsPermanentlyStocked() == true || shopTC->StockState == 1) && !open
                    && !keeper->GetAreOtherPeopleInShop(false)) {
                GoToInsideFace = shop == GetCreature()->GetPHome();
                Lock = true;
                return true;
            }
        }
    }
    return false;
}

int main()
{
    int ok = 1;
    CTCShop shopTC = { 0, true, false };
    CTCShopKeeper keeper = { &shopTC, false, 0 };
    CThingBuilding shop = { 0, 0, true, false, false, true, { false } };
    CThingBuilding other = { 0, 0, false, false, false, false, { false } };
    CThingShopkeeper me = { &shop, &other, &keeper, { 1.0f, 0.0f, 0.0f } };
    CAIBrain brain = { &me };
    CAIStateGroup_OpenAndCloseShop s = { &brain, false, false };
    // no occupants, open: sign opened, keeper flag set, not runnable
    shop.sign.m_closed = true;
    if (s.IsRunnable() || shop.sign.m_closed || !(shop.m_buildingFlags & 1)) ok = 0;
    // no occupants, shut: sign closed
    shopTC.openHours = false;
    if (s.IsRunnable() || !shop.sign.m_closed) ok = 0;
    // busy occupants, open: runnable, inside face from the position test
    shopTC.openHours = true; shop.occupants = true; shop.busy = true; shop.sign.m_closed = false;
    if (!s.IsRunnable() || !s.GoToInsideFace || s.Lock || shop.sign.m_closed) ok = 0;
    // busy, shut, inside != (shop == home): runnable without asking about other people
    shopTC.openHours = false; keeper.otherCalls = 0;
    if (!s.IsRunnable() || !s.GoToInsideFace || s.Lock || keeper.otherCalls || !shop.sign.m_closed) ok = 0;
    // busy, shut, inside == (shop == home): asks about other people (none -> not runnable)
    me.home = &shop;
    if (s.IsRunnable() || keeper.otherCalls != 1) ok = 0;
    keeper.othersInShop = true;
    if (!s.IsRunnable() || !s.GoToInsideFace) ok = 0;
    // idle occupants: lock-up path needs a permanent or state-1 stock, shut, nobody else
    shop.busy = false; keeper.othersInShop = false;
    if (s.IsRunnable()) ok = 0;
    shopTC.StockState = 1;
    if (!s.IsRunnable() || !s.Lock || !s.GoToInsideFace) ok = 0;
    me.home = &other; shopTC.StockState = 0; shopTC.permanent = true;
    if (!s.IsRunnable() || !s.Lock || s.GoToInsideFace) ok = 0;
    shopTC.openHours = true;
    if (s.IsRunnable()) ok = 0;
    // no keeper component / dead shop
    me.keeper = 0;
    if (s.IsRunnable()) ok = 0;
    me.keeper = &keeper; shop.m_flags91 = 1;
    if (s.IsRunnable()) ok = 0;
    if (!ok) { std::printf("00926c50_TEST FAIL\n"); return 1; }
    std::printf("00926c50_TEST PASS\n");
    return 0;
}