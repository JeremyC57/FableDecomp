#include <cstdio>
// Behaviour test for CTCSkeletalMorph scalar deleting destructor (0x007874C0): the counted pointer (+0x1c)
// dies first, then the morph array (+0x10), then the CTCBase base; the object is freed only for flag bit 0.
static int g_order[8]; static int g_n = 0;
struct CTCBase { virtual ~CTCBase() { g_order[g_n++] = 1; } long ParentThing; bool ToBeRemoved; bool IsInGlobalUpdateTCs; };
struct CSkeletalMorphArray { void* m_pData; long m_size; long m_capacity; ~CSkeletalMorphArray() { g_order[g_n++] = 2; } };
struct CCountedPointerSkeletalMorph { void* m_pObject; void* m_pCount; ~CCountedPointerSkeletalMorph() { g_order[g_n++] = 3; } };
struct CTCSkeletalMorph : CTCBase {
    bool Changed;
    CSkeletalMorphArray Morphs;
    CCountedPointerSkeletalMorph PSkeletalMorph;
    virtual ~CTCSkeletalMorph() { }
};
int main()
{
    CTCBase* p = new CTCSkeletalMorph;
    delete p;
    if (g_n != 3 || g_order[0] != 3 || g_order[1] != 2 || g_order[2] != 1) { std::printf("007874c0_TEST FAIL\n"); return 1; }
    g_n = 0;
    { CTCSkeletalMorph local; }
    if (g_n != 3 || g_order[0] != 3) { std::printf("007874c0_TEST FAIL\n"); return 1; }
    std::printf("007874c0_TEST PASS\n");
    return 0;
}