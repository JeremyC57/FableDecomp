// Behaviour test for CBrainDef::operator= (0x00462B15): base first, then the three vectors in order.
#include <cstdio>

static int g_order[4];
static int g_calls = 0;

struct CParentDefClassBase {
    virtual void v00() {}
    unsigned char _body[0x38];
    CParentDefClassBase& operator=(const CParentDefClassBase& other)
    {
        for (int i = 0; i < 0x38; ++i) _body[i] = other._body[i];
        g_order[g_calls++] = 1;
        return *this;
    }
};

struct CBrainBehaviourVector {
    void* m_first; void* m_last; void* m_end;
    CBrainBehaviourVector& operator=(const CBrainBehaviourVector& o)
    { m_first = o.m_first; m_last = o.m_last; m_end = o.m_end; g_order[g_calls++] = 2; return *this; }
};

struct CDefStringVector {
    void* m_first; void* m_last; void* m_end;
    CDefStringVector& operator=(const CDefStringVector& o)
    { m_first = o.m_first; m_last = o.m_last; m_end = o.m_end; g_order[g_calls++] = 3; return *this; }
};

struct CBrainUpdateZoneVector {
    void* m_first; void* m_last; void* m_end;
    CBrainUpdateZoneVector& operator=(const CBrainUpdateZoneVector& o)
    { m_first = o.m_first; m_last = o.m_last; m_end = o.m_end; g_order[g_calls++] = 4; return *this; }
};

struct CBrainDef : CParentDefClassBase {
    CBrainBehaviourVector  Behaviours;
    CDefStringVector       DisabledBehaviours;
    CBrainUpdateZoneVector UpdateZones;
    CBrainDef& operator=(const CBrainDef& other);
};

CBrainDef& CBrainDef::operator=(const CBrainDef& other)
{
    CParentDefClassBase::operator=(other);
    Behaviours = other.Behaviours;
    DisabledBehaviours = other.DisabledBehaviours;
    UpdateZones = other.UpdateZones;
    return *this;
}

int main()
{
    static int x, y, z;
    CBrainDef a, b;
    for (int i = 0; i < 0x38; ++i) { a._body[i] = 0; b._body[i] = 9; }
    a.Behaviours.m_first = a.Behaviours.m_last = a.Behaviours.m_end = 0;
    a.DisabledBehaviours.m_first = a.DisabledBehaviours.m_last = a.DisabledBehaviours.m_end = 0;
    a.UpdateZones.m_first = a.UpdateZones.m_last = a.UpdateZones.m_end = 0;
    b.Behaviours.m_first = &x; b.Behaviours.m_last = &x; b.Behaviours.m_end = &x;
    b.DisabledBehaviours.m_first = &y; b.DisabledBehaviours.m_last = &y; b.DisabledBehaviours.m_end = &y;
    b.UpdateZones.m_first = &z; b.UpdateZones.m_last = &z; b.UpdateZones.m_end = &z;
    CBrainDef& r = (a = b);
    if (&r != &a || g_calls != 4 || g_order[0] != 1 || g_order[1] != 2 || g_order[2] != 3 || g_order[3] != 4
        || a._body[5] != 9 || a.Behaviours.m_first != &x || a.DisabledBehaviours.m_last != &y || a.UpdateZones.m_end != &z) {
        std::printf("00462b15_TEST FAIL\n");
        return 1;
    }
    std::printf("00462b15_TEST PASS\n");
    return 0;
}
