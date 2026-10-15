// CTCInventoryBase::GetCategoryWithAssociationThing(CThing* thing) -- returns the first category whose associated thing is `thing`, or null
// Grade: DIFFER(84v85) -- register allocation only: retail keeps the 0x2C stride offset in ebx and spills the element count to
// [ebp-4] (`lea ecx,[ebx+eax]`, `cmp edi,[ebp-4]`); the lane keeps the count in ebx and spills the offset (`add ecx,[ebp-4]`,
// `add dword ptr [ebp-4],2Ch`). Same instructions otherwise.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005BD5DB, length 85 bytes. Frame-pointer size TU: optimize("y",off) + ("s",on).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 on each CCategory's PAssociatedThing (+0).
// Retail CTCInventoryBase: no engine header; Categories (0x2C-byte CCategory) at +0x20. The loop compares signed (`jl`/`jge`):
// the element count is a long local; the found element is rebuilt after the loop as `imul edi,2Ch ; add edi,[esi+20h]`.
// Tried: for-break + post `if (i < count)` (this file), a while loop with i/count/result declared in reverse order, a for loop
// with the `&&` condition and `first + i`, and an outer `if (count > 0)` (worse: esi/edi swap).
#pragma optimize("y", off)
#pragma optimize("s", on)
struct CThing;
struct CBaseIntelligentPointer {
    void* m_a; void* m_b;
    CThing* GetPItem() const;                    // 0x00A01B50
};
struct CCategory_Retail {
    CBaseIntelligentPointer PAssociatedThing;    // +0x00
    unsigned char _body[0x24];
};
struct CCategoryVector {
    CCategory_Retail* first;
    CCategory_Retail* last;
    CCategory_Retail* end_of_storage;
    unsigned long size() const { return (unsigned long)(last - first); }
    CCategory_Retail& operator[](unsigned long n) { return first[n]; }
};
struct CTCInventoryBase_Retail {
    unsigned char   _pad_0x00[0x20];
    CCategoryVector Categories;                  // +0x20
    CCategory_Retail* GetCategoryWithAssociationThing(CThing* thing);
};
CCategory_Retail* CTCInventoryBase_Retail::GetCategoryWithAssociationThing(CThing* thing)
{
    CCategory_Retail* result = 0;
    long count = Categories.size();
    long i;
    for (i = 0; i < count; ++i)
        if (Categories[i].PAssociatedThing.GetPItem() == thing)
            break;
    if (i < count)
        result = &Categories[i];
    return result;
}
