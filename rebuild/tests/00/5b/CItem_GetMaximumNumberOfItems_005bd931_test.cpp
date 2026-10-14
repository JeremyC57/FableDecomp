#include <cstdio>
#include <cstdlib>
#include <cstring>
static char g_seq[64] = "";
struct CIVCountedPointeeBase { virtual void Destroy(int) { } virtual void Delete() { std::strcat(g_seq, "D"); } unsigned long IVRefCount; }; struct CDef : CIVCountedPointeeBase { unsigned char _pad[0x40 - sizeof(CIVCountedPointeeBase)]; long MaxItems; }; static CDef g_def;
struct CIVCountedPointer_Def { CDef* Object; CIVCountedPointer_Def() : Object(0) { } CIVCountedPointer_Def(const CIVCountedPointer_Def& o) : Object(o.Object) { if (Object) ++Object->IVRefCount; } ~CIVCountedPointer_Def() { CDef* old = Object; if (old) { CDef* object = old; unsigned long* count = &old->IVRefCount; --*count; if (*(volatile unsigned long*)count == 0) object->Delete(); } } };
struct CItem { long ItemType; long NumberOfItems; void* PPerItemData_Data; void* PPerItemData_Info; long AverageBoughtPrice;
    CIVCountedPointer_Def GetInventoryItemDef() { std::strcat(g_seq, "G"); CIVCountedPointer_Def p; p.Object = &g_def; ++g_def.IVRefCount; return p; }
    long GetMaximumNumberOfItems() { if (NumberOfItems == 0) return 0; long maximum = GetInventoryItemDef().Object->MaxItems; return maximum; }
    bool IsSlotFull() { if (NumberOfItems == 0) return false; long maximum = GetInventoryItemDef().Object->MaxItems; return NumberOfItems == maximum; } };

int main() {
    CItem i; i.NumberOfItems = 0; g_def.MaxItems = 9; g_def.IVRefCount = 0; if (i.GetMaximumNumberOfItems() != 0 || g_seq[0]) { std::printf("005bd931_TEST FAIL\n"); return 1; } i.NumberOfItems = 2; if (i.GetMaximumNumberOfItems() != 9 || std::strcmp(g_seq, "GD") || g_def.IVRefCount != 0) { std::printf("005bd931_TEST FAIL\n"); return 1; }
    g_seq[0] = 0; g_def.IVRefCount = 1; if (i.GetMaximumNumberOfItems() != 9 || std::strcmp(g_seq, "G") || g_def.IVRefCount != 1) { std::printf("005bd931_TEST FAIL\n"); return 1; }
    std::printf("005bd931_TEST PASS\n"); return 0;
}