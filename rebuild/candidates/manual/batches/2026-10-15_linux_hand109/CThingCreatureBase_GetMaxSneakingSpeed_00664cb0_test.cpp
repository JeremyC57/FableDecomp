// Behaviour test for CThingCreatureBase::GetMaxSneakingSpeed (0x00664CB0)
#include <cstdio>
struct CTCBase;
struct STCEntry { long m_kind; CTCBase* m_pTC; };
struct CTCMap {
    STCEntry* m_pFirst; STCEntry* m_pLast; STCEntry* m_pEnd;
    STCEntry* LowerBound(long kind) const
    {
        STCEntry* it = m_pFirst;
        while (it != m_pLast && it->m_kind < kind) ++it;
        return it;
    }
};
struct CThing {
    unsigned long m_tcMask[8];
    CTCMap m_tcs;
    unsigned long HasTC(long kind) const { return m_tcMask[kind >> 5] & (1UL << (kind & 31)); }
    CTCBase* GetTC(long kind) const
    {
        STCEntry* it = m_tcs.LowerBound(kind);
        if (it == m_tcs.m_pLast || kind < it->m_kind)
            it = m_tcs.m_pLast;
        return it->m_pTC;
    }
    template<class T> bool GetInterface(T** ppOut, long kind) const
    {
        if (HasTC(kind)) { *ppOut = (T*)GetTC(kind); return true; }
        return false;
    }
    void Clear() { for (int i = 0; i < 8; ++i) m_tcMask[i] = 0; }
    void Add(long kind) { m_tcMask[kind >> 5] |= 1UL << (kind & 31); }
};


struct C3DVector { float x, y, z; };
struct CTCSneakMovement { float m_speed; float GetMaxSneakingSpeed() const { return m_speed; } };
struct CThingCreatureBase : CThing {
    float GetMaxSneakingSpeed(const C3DVector& direction) const
    {
        if (HasTC(0x36))
            return ((CTCSneakMovement*)GetTC(0x36))->GetMaxSneakingSpeed();
        return 0.0f;
    }
};
#define CHECK(c) if (!(c)) { std::printf("00664cb0_TEST FAIL\n"); return 1; }
int main()
{
    CTCSneakMovement m = { 2.5f }, end = { 9.0f };
    STCEntry e[2] = { { 0x36, (CTCBase*)&m }, { 0, (CTCBase*)&end } };
    CThingCreatureBase c; c.Clear(); c.m_tcs.m_pFirst = e; c.m_tcs.m_pLast = e + 1; c.m_tcs.m_pEnd = e + 2;
    C3DVector dir = { 1.0f, 0.0f, 0.0f };
    CHECK(c.GetMaxSneakingSpeed(dir) == 0.0f);
    c.Add(0x36);
    CHECK(c.GetMaxSneakingSpeed(dir) == 2.5f);
    std::printf("00664cb0_TEST PASS\n");
    return 0;
}
