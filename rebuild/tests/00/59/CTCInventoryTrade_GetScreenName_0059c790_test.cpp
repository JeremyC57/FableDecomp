// behaviour test for 0x0059C790 CTCInventoryTrade::GetScreenName() const: the title is the text of the def's screen-name tag, the def is held (one extra reference) only while the text is fetched, and the last release destroys the def
#include <cstdio>
struct CDef {
    long RefCount; long ScreenNameTag; int Destroyed;
    void Destroy() { ++Destroyed; }
};
struct CDefPtr {
    CDef* Object;
    CDefPtr(const CDefPtr& o) : Object(o.Object) { if (Object) ++Object->RefCount; }
    explicit CDefPtr(CDef* d) : Object(d) { }
    ~CDefPtr() { if (--Object->RefCount == 0) Object->Destroy(); }
    CDef* operator->() const { return Object; }
};
static long gRefDuringFetch = 0;
struct CInventory {
    CDefPtr PDef;
    explicit CInventory(CDef* d) : PDef(d) { }
    int GetInventoryText(long tag) const { gRefDuringFetch = PDef.Object->RefCount; return (int)tag * 10; }
    int GetScreenName() const                       // the function under test
    {
        CDefPtr def = PDef;
        return GetInventoryText(def->ScreenNameTag);
    }
};
int main()
{
    int fail = 0;
    CDef d; d.RefCount = 1; d.ScreenNameTag = 42; d.Destroyed = 0;
    {
        CInventory inv(&d);
        if (inv.GetScreenName() != 420) fail |= 1;
        if (gRefDuringFetch != 2 || d.RefCount != 1 || d.Destroyed != 0) fail |= 2;
        CDef d2; d2.RefCount = 0; d2.ScreenNameTag = 7; d2.Destroyed = 0;
        CInventory last(&d2);
        d2.RefCount = 0;
        if (last.GetScreenName() != 70 || d2.Destroyed != 1) fail |= 4;
        d2.RefCount = 1; d.RefCount = 1;             // let the members' own releases run harmlessly
    }
    if (fail) { std::printf("0059c790_TEST FAIL %d\n", fail); return 1; }
    std::printf("0059c790_TEST PASS\n");
    return 0;
}