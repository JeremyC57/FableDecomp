#include <cstdio>

static int g_render = 0;
struct CThingBaseDef { unsigned long PersistenceFlags; };
struct CThing { CThingBaseDef* PBaseDef; void AddToRenderList() { ++g_render; } };
struct CTCGraphicAppearance {
    CThing* ParentThing;
    unsigned short ActivePersistenceFlags;
    unsigned char UpdatePrimitiveReasons;
    void SetActivePersistenceFlags(unsigned long flags)
    {
        if (!flags)
            flags = ParentThing->PBaseDef->PersistenceFlags;
        if (ActivePersistenceFlags != flags) {
            ActivePersistenceFlags = (unsigned short)flags;
            ParentThing->AddToRenderList();
            UpdatePrimitiveReasons |= 4;
        }
    }
};

int main()
{
    CThingBaseDef def; def.PersistenceFlags = 0x30;
    CThing t; t.PBaseDef = &def;
    CTCGraphicAppearance a; a.ParentThing = &t; a.ActivePersistenceFlags = 0x30; a.UpdatePrimitiveReasons = 0;
    a.SetActivePersistenceFlags(0);                      // default equals current: nothing
    if (g_render != 0 || a.UpdatePrimitiveReasons != 0) { std::printf("004bffd0_TEST FAIL\n"); return 1; }
    a.SetActivePersistenceFlags(5);
    if (a.ActivePersistenceFlags != 5 || g_render != 1 || a.UpdatePrimitiveReasons != 4) { std::printf("004bffd0_TEST FAIL\n"); return 1; }
    a.SetActivePersistenceFlags(0);                      // back to the def's flags
    if (a.ActivePersistenceFlags != 0x30 || g_render != 2) { std::printf("004bffd0_TEST FAIL\n"); return 1; }
    std::printf("004bffd0_TEST PASS\n");
    return 0;
}