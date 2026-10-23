#include <cstdio>
#include <cstdlib>
#include <cstring>
static char g_seq[64] = "";
struct CIVCountedPointeeVirtual { virtual void Destroy(int) { } virtual void Delete() { std::strcat(g_seq, "D"); } };
struct CIVRefCounted { unsigned long IVRefCount; void DecRef() { --IVRefCount; } unsigned long GetRefCount() const { return IVRefCount; } };
struct CIVCountedPointeeBase : CIVCountedPointeeVirtual, CIVRefCounted { void ReleaseTemporary() { DecRef(); if (GetRefCount() == 0) Delete(); } }; struct CDef : CIVCountedPointeeBase { unsigned char _pad[0x40 - sizeof(CIVCountedPointeeBase)]; long MaxItems; }; static CDef g_def;
struct CIVCountedPointer_Def { CDef* Object; CIVCountedPointer_Def() : Object(0) { } CIVCountedPointer_Def(const CIVCountedPointer_Def& o) : Object(o.Object) { if (Object) ++Object->IVRefCount; } ~CIVCountedPointer_Def() { if (Object) Object->ReleaseTemporary(); } };
struct CItem { long ItemType; long NumberOfItems; void* PPerItemData_Data; void* PPerItemData_Info; long AverageBoughtPrice;
    CIVCountedPointer_Def GetInventoryItemDef() { std::strcat(g_seq, "G"); CIVCountedPointer_Def p; p.Object = &g_def; ++g_def.IVRefCount; return p; }
    long GetMaximumNumberOfItems() { if (NumberOfItems == 0) return 0; long maximum = GetInventoryItemDef().Object->MaxItems; return maximum; }
    bool IsSlotFull() { if (NumberOfItems == 0) return false; long maximum = GetInventoryItemDef().Object->MaxItems; return NumberOfItems == maximum; } };

int main() {
    CItem i; i.NumberOfItems = 0; g_def.MaxItems = 9; g_def.IVRefCount = 0; if (i.IsSlotFull() || g_seq[0]) { std::printf("005bde50_TEST FAIL\n"); return 1; } i.NumberOfItems = 9; if (!i.IsSlotFull() || std::strcmp(g_seq, "GD")) { std::printf("005bde50_TEST FAIL\n"); return 1; } g_seq[0] = 0; i.NumberOfItems = 8; if (i.IsSlotFull() || std::strcmp(g_seq, "GD")) { std::printf("005bde50_TEST FAIL\n"); return 1; }
    std::printf("005bde50_TEST PASS\n"); return 0;
}