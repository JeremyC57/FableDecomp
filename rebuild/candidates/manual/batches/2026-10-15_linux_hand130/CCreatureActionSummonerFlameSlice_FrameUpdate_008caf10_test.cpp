// Behaviour test for 0x008CAF10 CCreatureAction_SummonerFlameSlice::FrameUpdate: the obstruction is stretched over the summoner.
#include <cstdio>
static int g_base = 0;
struct C3DVector { float X, Y, Z; };
struct CTCPlacement { C3DVector m_position; float m_radius; };
struct CObstructionShape {
    C3DVector m_start, m_end; float m_width; int m_sets;
    void Set(const C3DVector& start, const C3DVector& end, float width) { m_start = start; m_end = end; m_width = width; ++m_sets; }
};
struct CThing {
    unsigned int m_presentBits[9];
    CObstructionShape* m_pObstruction;
    CTCPlacement* m_pPlacement;
    unsigned char m_flags91;
    const C3DVector& GetPosition() const { return m_pPlacement->m_position; }
    unsigned int HasTC(int t) const { return m_presentBits[t >> 5] & (1u << (t & 31)); }
};
static void SetBetween(CObstructionShape* o, CThing* start, CThing* end, CTCPlacement* p) { o->Set(start->GetPosition(), end->GetPosition(), p->m_radius * 2.0f); }
struct CBaseIntelligentPointer { CThing* m_pItem; CThing* GetPItem() const { return m_pItem; } };
inline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
{
    if (p.GetPItem() != 0) { if (!(p.GetPItem()->m_flags91 & 1)) return true; }
    return false;
}
struct CAction {
    CBaseIntelligentPointer PCreature, PFlameSlice;
    void FrameUpdate()
    {
        ++g_base;
        if (IS_THING_ALIVE(PFlameSlice)) {
            CThing* slice = PFlameSlice.GetPItem();
            if (slice->HasTC(0xed)) {
                CObstructionShape* obstruction = slice->m_pObstruction;
                SetBetween(obstruction, PCreature.GetPItem(), PCreature.GetPItem(), PCreature.GetPItem()->m_pPlacement);
            }
        }
    }
};
static int Fail() { std::printf("008caf10_TEST FAIL\n"); return 1; }
int main()
{
    CTCPlacement place; place.m_position.X = 1; place.m_position.Y = 2; place.m_position.Z = 3; place.m_radius = 1.25f;
    CThing me; me.m_pPlacement = &place;
    CObstructionShape shape; shape.m_sets = 0; shape.m_width = 0;
    CThing slice; for (int i = 0; i < 9; ++i) slice.m_presentBits[i] = 0;
    slice.m_pObstruction = &shape; slice.m_flags91 = 0; slice.m_pPlacement = 0;
    CAction a; a.PCreature.m_pItem = &me; a.PFlameSlice.m_pItem = &slice;
    a.FrameUpdate();
    if (shape.m_sets != 0 || g_base != 1) return Fail();         // no obstruction component yet
    slice.m_presentBits[0xed >> 5] |= 1u << (0xed & 31);
    a.FrameUpdate();
    if (shape.m_sets != 1 || shape.m_width != 2.5f || shape.m_start.Y != 2 || shape.m_end.Z != 3) return Fail();
    slice.m_flags91 = 1; a.FrameUpdate();
    if (shape.m_sets != 1 || g_base != 3) return Fail();
    std::printf("008caf10_TEST PASS\n");
    return 0;
}
