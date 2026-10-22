#include <cstdio>

struct CStockItemDef { long RefCount; int destroyed; long DefaultPrice; bool DefaultIsStealable; void DestroySelf() { ++destroyed; } };
struct CThingDef { CStockItemDef* sub; void GetSubDef(CStockItemDef** out) { *out = sub; ++sub->RefCount; } };
struct CThing { CThingDef* PDef; };
struct CTCStockItem {
    CThing* thing; bool Stealable; bool Initialised; long Price;
    CThing* GetThing() { return thing; }
    void OnCreate()
    {
        if (!Initialised) {
            CStockItemDef* object = 0;
            GetThing()->PDef->GetSubDef(&object);
            Price = object->DefaultPrice;
            Stealable = object->DefaultIsStealable;
            Initialised = true;
            if (--object->RefCount == 0)
                object->DestroySelf();
        }
    }
};
static bool Run()
{
    CStockItemDef d; d.RefCount = 0; d.destroyed = 0; d.DefaultPrice = 150; d.DefaultIsStealable = true;
    CThingDef td; td.sub = &d; CThing t; t.PDef = &td;
    CTCStockItem s; s.thing = &t; s.Stealable = false; s.Initialised = false; s.Price = 0;
    s.OnCreate();
    bool ok = s.Price == 150 && s.Stealable && s.Initialised && d.destroyed == 1 && d.RefCount == 0;
    d.DefaultPrice = 1; s.OnCreate();
    return ok && s.Price == 150 && d.destroyed == 1;
}

int main()
{
    if (!Run()) { std::printf("007f4e80_TEST FAIL\n"); return 1; }
    std::printf("007f4e80_TEST PASS\n");
    return 0;
}