// Behaviour test for 00c1eaa0: CUIComponentBase::CUIComponentBase(owner, rect) -- owner, rect copy, state words (1, 0), then the element appends itself to the owner's element array
#include <cstdio>

struct SUIRect { long m_words[5]; };
struct CUIComponentBase;

static int g_grows = 0;
template <class T>
struct CPtrArray {
    T* m_data;
    unsigned int m_capacity;
    unsigned int m_count;
    void Grow(unsigned int slots) { ++g_grows; m_capacity += slots; }
    void Add(T value)
    {
        if (m_capacity - m_count == 0)
            Grow(1);
        m_data[m_count] = value;
        ++m_count;
    }
};

struct CUIOwner { CPtrArray<CUIComponentBase*> m_elements; };

struct CUIComponentBase {
    virtual void v00() { }
    CUIOwner* m_pOwner;
    SUIRect m_rect;
    unsigned short m_state;
    unsigned short m_flags;
    CUIComponentBase(CUIOwner* owner, const SUIRect& rect)
        : m_pOwner(owner), m_rect(rect), m_state(1), m_flags(0)
    {
        owner->m_elements.Add(this);
    }
};

int main()
{
    CUIComponentBase* store[4];
    CUIOwner owner; owner.m_elements.m_data = store; owner.m_elements.m_capacity = 1; owner.m_elements.m_count = 0;
    SUIRect r = { { 10, 20, 30, 40, 50 } };
    CUIComponentBase a(&owner, r);
    r.m_words[0] = 99;
    CUIComponentBase b(&owner, r);
    if (owner.m_elements.m_count != 2 || store[0] != &a || store[1] != &b || g_grows != 1) { std::printf("00c1eaa0_TEST FAIL\n"); return 1; }
    if (a.m_pOwner != &owner || a.m_rect.m_words[0] != 10 || a.m_rect.m_words[4] != 50 || b.m_rect.m_words[0] != 99) { std::printf("00c1eaa0_TEST FAIL\n"); return 1; }
    if (a.m_state != 1 || a.m_flags != 0) { std::printf("00c1eaa0_TEST FAIL\n"); return 1; }
    std::printf("00c1eaa0_TEST PASS\n");
    return 0;
}