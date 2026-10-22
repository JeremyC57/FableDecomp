#include <cstdio>
// Behaviour test for 007f4f40: GetSellersBanter fetches the CStockItemDef (one reference taken by GetSubDef),
// reads SellersBanter, then drops the reference (destroying the def when it was the last one).
struct CStockItemDef {
    long RefCount; unsigned long SellersBanter; int destroyed;
    void DestroySelf() { ++destroyed; }
};
struct CDefClassBase {
    CStockItemDef* def;
    bool GetSubDef(CStockItemDef** out) { ++def->RefCount; *out = def; return true; }
};
struct CThing { CDefClassBase* PDef; };
struct CTCStockItem {
    CThing* Parent;
    unsigned long GetSellersBanter()
    {
        CStockItemDef* object = 0;
        Parent->PDef->GetSubDef(&object);
        unsigned long banter = object->SellersBanter;
        if (--object->RefCount == 0) object->DestroySelf();
        return banter;
    }
};
int main()
{
    CStockItemDef d; d.RefCount = 1; d.SellersBanter = 0x1234; d.destroyed = 0;
    CDefClassBase base; base.def = &d;
    CThing t; t.PDef = &base;
    CTCStockItem item; item.Parent = &t;
    int ok = (item.GetSellersBanter() == 0x1234 && d.RefCount == 1 && d.destroyed == 0);
    d.RefCount = 0;
    ok = ok && item.GetSellersBanter() == 0x1234 && d.destroyed == 1;
    if (!ok) { std::printf("007f4f40_TEST FAIL\n"); return 1; }
    std::printf("007f4f40_TEST PASS\n");
    return 0;
}
