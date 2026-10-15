// Behaviour test for CTCObjectAugmentations::GetFreeSlot @ 0x00765D50: first null slot index, -1 when full or empty.
#include <cstdio>
struct CDefPtr { const void* m_pDef; };
struct CAugmentationVector {
    CDefPtr* m_pFirst; CDefPtr* m_pLast;
    unsigned long size() const { return m_pLast - m_pFirst; }
    const CDefPtr& operator[](unsigned long i) const { return m_pFirst[i]; }
};
struct CTCObjectAugmentations {
    CAugmentationVector Augmentations;
    long GetFreeSlot() const
    {
        for (unsigned long i = 0; i < Augmentations.size(); ++i) if (Augmentations[i].m_pDef == 0) return i;
        return -1;
    }
};
int main()
{
    int x;
    CDefPtr slots[3] = { { &x }, { 0 }, { 0 } };
    CTCObjectAugmentations a; a.Augmentations.m_pFirst = slots; a.Augmentations.m_pLast = slots + 3;
    if (a.GetFreeSlot() != 1) { std::printf("00765d50_TEST FAIL\n"); return 1; }
    slots[1].m_pDef = &x; slots[2].m_pDef = &x;
    if (a.GetFreeSlot() != -1) { std::printf("00765d50_TEST FAIL\n"); return 1; }
    a.Augmentations.m_pLast = slots;
    if (a.GetFreeSlot() != -1) { std::printf("00765d50_TEST FAIL\n"); return 1; }
    std::printf("00765d50_TEST PASS\n");
    return 0;
}