// Behaviour model of ??_GCItem@NInventory@@QAEPAXI@Z 0x005C1390: destroy the item (its CCountedPointer<NInventory::CPerItemData>
// PPerItemData, after ItemType / NumberOfItems), then free the storage only when bit 0 of the flags is set; returns this.
#include <cstdio>
#include <cstdlib>
#include <new>
static int g_dtor_calls = 0, g_delete_calls = 0;
static const void* g_last_dtor_self = 0;
static const void* g_last_delete_self = 0;
struct CPerItemDataPointer { void* Data; void* Info; ~CPerItemDataPointer() { g_dtor_calls++; g_last_dtor_self = this; } };
struct CItem {
    int ItemType; int NumberOfItems; CPerItemDataPointer PPerItemData; int AverageBoughtPrice;
    static void operator delete(void* p) { g_delete_calls++; g_last_delete_self = p; std::free(p); }
};
static void* ScalarDeletingDestructor(CItem* self, unsigned int flags)
{
    self->~CItem();
    if (flags & 1)
        CItem::operator delete(self);
    return self;
}
int main()
{
    void* mem = std::malloc(sizeof(CItem));
    CItem* p = new (mem) CItem;
    void* r = ScalarDeletingDestructor(p, 0);
    if (r != p || g_dtor_calls != 1 || g_delete_calls != 0) { std::printf("FAIL flag0\n"); return 1; }
    if (g_last_dtor_self != &p->PPerItemData) { std::printf("FAIL member\n"); return 1; }
    p = new (mem) CItem;
    r = ScalarDeletingDestructor(p, 2);
    if (r != p || g_dtor_calls != 2 || g_delete_calls != 0) { std::printf("FAIL flag2\n"); return 1; }
    p = new (mem) CItem;
    r = ScalarDeletingDestructor(p, 1);
    if (r != p || g_dtor_calls != 3 || g_delete_calls != 1 || g_last_delete_self != p) { std::printf("FAIL flag1\n"); return 1; }
    std::printf("PASS_005c1390\n");
    return 0;
}
