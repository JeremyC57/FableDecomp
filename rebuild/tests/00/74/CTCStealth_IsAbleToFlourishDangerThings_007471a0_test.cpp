// Behaviour test for CTCStealth::IsAbleToFlourishDangerThings (0x007471A0).
#include <cstdio>

struct CThing { bool m_knockedDown; };
bool g_FlourishDangerThings = true;
bool IsKnockedDown(CThing* thing) { return thing->m_knockedDown; }

struct CDangerThing {
    struct CPointer {
        CThing* m_pItem;
        CThing* GetPItem() const { return m_pItem; }
    } first;
    long Status;
    long Timer;
};

struct CTCStealth {
    CDangerThing* m_pDangerFirst;
    CDangerThing* m_pDangerLast;
    bool IsAbleToFlourishDangerThings() const;
};

bool CTCStealth::IsAbleToFlourishDangerThings() const
{
    if (!g_FlourishDangerThings)
        return true;
    for (CDangerThing* it = m_pDangerFirst; it != m_pDangerLast; ++it) {
        CThing* thing = it->first.GetPItem();
        if (thing && !IsKnockedDown(thing) && it->Status != 4)
            return true;
    }
    return false;
}

int main()
{
    CThing down = { true }, up = { false };
    CDangerThing d[3];
    d[0].first.m_pItem = 0;     d[0].Status = 1;
    d[1].first.m_pItem = &down; d[1].Status = 1;
    d[2].first.m_pItem = &up;   d[2].Status = 4;
    CTCStealth s; s.m_pDangerFirst = d; s.m_pDangerLast = d + 3;
    bool r1 = s.IsAbleToFlourishDangerThings();             // nobody qualifies
    d[2].Status = 2;
    bool r2 = s.IsAbleToFlourishDangerThings();             // the standing thing qualifies
    s.m_pDangerLast = d;
    bool r3 = s.IsAbleToFlourishDangerThings();             // empty
    g_FlourishDangerThings = false;
    bool r4 = s.IsAbleToFlourishDangerThings();             // switch off: always true
    if (r1 || !r2 || r3 || !r4) { std::printf("007471a0_TEST FAIL\n"); return 1; }
    std::printf("007471a0_TEST PASS\n");
    return 0;
}