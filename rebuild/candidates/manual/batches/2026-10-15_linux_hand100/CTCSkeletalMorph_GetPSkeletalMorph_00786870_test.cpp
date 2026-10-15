#include <cstdio>
struct CSkeletalMorph { int id; };
struct CCountedPointer_CSkeletalMorph {
    CSkeletalMorph* m_pObject; long* m_pCount;
    CCountedPointer_CSkeletalMorph(CSkeletalMorph* o, long* c) : m_pObject(o), m_pCount(c) { }
    CCountedPointer_CSkeletalMorph(const CCountedPointer_CSkeletalMorph& other) : m_pObject(other.m_pObject), m_pCount(other.m_pCount)
    {
        if (m_pCount)
            ++*m_pCount;
    }
};
static int g_calcs = 0;
struct CTCSkeletalMorph {
    bool m_dirty; CCountedPointer_CSkeletalMorph m_pSkeletalMorph;
    CTCSkeletalMorph(CSkeletalMorph* o, long* c) : m_dirty(true), m_pSkeletalMorph(o, c) { }
    void CalculateSkeletalMorph() { ++g_calcs; }
    CCountedPointer_CSkeletalMorph GetPSkeletalMorph()
    {
        if (m_dirty) {
            CalculateSkeletalMorph();
            m_dirty = false;
        }
        return m_pSkeletalMorph;
    }
};
int main()
{
    CSkeletalMorph morph; long count = 1;
    CTCSkeletalMorph tc(&morph, &count);
    CCountedPointer_CSkeletalMorph a = tc.GetPSkeletalMorph();
    CCountedPointer_CSkeletalMorph b = tc.GetPSkeletalMorph();
    if (a.m_pObject != &morph || b.m_pCount != &count || count < 3 || g_calcs != 1 || tc.m_dirty) { std::printf("00786870_TEST FAIL\n"); return 1; }
    CTCSkeletalMorph empty(0, 0);
    CCountedPointer_CSkeletalMorph c = empty.GetPSkeletalMorph();
    if (c.m_pObject != 0 || c.m_pCount != 0) { std::printf("00786870_TEST FAIL\n"); return 1; }
    std::printf("00786870_TEST PASS\n");
    return 0;
}
