// Behaviour test for CPlayerInventoryDef::operator= (0x00462E2D).
#include <cstdio>

static int g_order[8];
static int g_calls = 0;

class CParentDefClassBase { public:
    virtual void v00() {}
    unsigned char _body[0x38];
    CParentDefClassBase& operator=(const CParentDefClassBase& other)
    {
        for (int i = 0; i < 0x38; ++i) _body[i] = other._body[i];
        g_order[g_calls++] = 1;
        return *this;
    }
};

struct CLongVector {
    long* m_first; long* m_last; long* m_end;
    CLongVector& operator=(const CLongVector& o)
    { m_first = o.m_first; m_last = o.m_last; m_end = o.m_end; g_order[g_calls++] = 2; return *this; }
};

struct CBoolVector {
    unsigned long* m_words; unsigned long m_size; unsigned long m_capacity;
    CBoolVector& operator=(const CBoolVector& o)
    { m_words = o.m_words; m_size = o.m_size; m_capacity = o.m_capacity; g_order[g_calls++] = 3; return *this; }
};

class CPlayerInventoryDef : public CParentDefClassBase { public:
    CLongVector Slot;
    CLongVector MaxInSlot;
    CLongVector InitialAmount;
    CBoolVector IsUsable;
    long        InitialSelectedSlot;
    CPlayerInventoryDef& operator=(const CPlayerInventoryDef& other);
};

CPlayerInventoryDef& CPlayerInventoryDef::operator=(const CPlayerInventoryDef& other)
{
    CParentDefClassBase::operator=(other);
    Slot = other.Slot;
    MaxInSlot = other.MaxInSlot;
    InitialAmount = other.InitialAmount;
    IsUsable = other.IsUsable;
    InitialSelectedSlot = other.InitialSelectedSlot;
    return *this;
}

int main()
{
    static long s[2], m[2], n[2];
    static unsigned long w[1];
    CPlayerInventoryDef a, b;
    for (int i = 0; i < 0x38; ++i) { a._body[i] = 0; b._body[i] = 1; }
    a.Slot.m_first = a.Slot.m_last = a.Slot.m_end = 0;
    a.MaxInSlot.m_first = a.MaxInSlot.m_last = a.MaxInSlot.m_end = 0;
    a.InitialAmount.m_first = a.InitialAmount.m_last = a.InitialAmount.m_end = 0;
    a.IsUsable.m_words = 0; a.IsUsable.m_size = 0; a.IsUsable.m_capacity = 0;
    a.InitialSelectedSlot = 0;
    b.Slot.m_first = s; b.Slot.m_last = s + 2; b.Slot.m_end = s + 2;
    b.MaxInSlot.m_first = m; b.MaxInSlot.m_last = m + 2; b.MaxInSlot.m_end = m + 2;
    b.InitialAmount.m_first = n; b.InitialAmount.m_last = n + 2; b.InitialAmount.m_end = n + 2;
    b.IsUsable.m_words = w; b.IsUsable.m_size = 5; b.IsUsable.m_capacity = 32;
    b.InitialSelectedSlot = 3;
    CPlayerInventoryDef& r = (a = b);
    if (&r != &a || g_calls != 5 || g_order[0] != 1 || g_order[1] != 2 || g_order[2] != 2 || g_order[3] != 2
        || g_order[4] != 3 || a._body[0] != 1 || a.Slot.m_first != s || a.MaxInSlot.m_last != m + 2
        || a.InitialAmount.m_end != n + 2 || a.IsUsable.m_size != 5 || a.InitialSelectedSlot != 3) {
        std::printf("00462e2d_TEST FAIL\n");
        return 1;
    }
    std::printf("00462e2d_TEST PASS\n");
    return 0;
}