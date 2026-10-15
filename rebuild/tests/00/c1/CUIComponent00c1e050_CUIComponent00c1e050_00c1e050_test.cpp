#include <cstdio>
// Behaviour test for 00c1e050: the base is built with (owner, rect), the second and third arguments land in the two
// derived members, and the object has the derived dynamic type.
struct CUIOwner_00c1e050 { int m_registered; };
struct SUIRect { long m_words[5]; };
struct CUIComponentBase_00c1e050 {
    CUIOwner_00c1e050* m_pOwner;
    SUIRect            m_rect;
    CUIComponentBase_00c1e050(CUIOwner_00c1e050* owner, const SUIRect& rect) : m_pOwner(owner), m_rect(rect) { ++owner->m_registered; }
    virtual ~CUIComponentBase_00c1e050() {}
    virtual int Kind() const { return 1; }
};
struct CUIComponent_00c1e050 : CUIComponentBase_00c1e050 {
    void* m_pSecond;
    void* m_pThird;
    CUIComponent_00c1e050(CUIOwner_00c1e050* owner, void* second, void* third, const SUIRect& rect);
    virtual int Kind() const { return 2; }
};
CUIComponent_00c1e050::CUIComponent_00c1e050(CUIOwner_00c1e050* owner, void* second, void* third, const SUIRect& rect)
    : CUIComponentBase_00c1e050(owner, rect), m_pSecond(second), m_pThird(third)
{
}
int main()
{
    CUIOwner_00c1e050 owner; owner.m_registered = 0;
    SUIRect rect; for (int i = 0; i < 5; ++i) rect.m_words[i] = 10 + i;
    int a = 0, b = 0;
    CUIComponent_00c1e050 c(&owner, &a, &b, rect);
    CUIComponentBase_00c1e050* base = &c;
    if (owner.m_registered != 1 || c.m_pOwner != &owner || c.m_rect.m_words[4] != 14 || c.m_pSecond != &a || c.m_pThird != &b || base->Kind() != 2) {
        std::printf("00c1e050_TEST FAIL\n"); return 1;
    }
    std::printf("00c1e050_TEST PASS\n");
    return 0;
}