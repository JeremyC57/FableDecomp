// behaviour copy of NCreatureMode::CModeIceTrollGroundSmashIcicleAssault::OnDeactivate: every alive icicle trap is set to type 4 and
// reset, every entry is erased from the front
#include <cstdio>
static int g_base = 0, g_resume = -1, g_reset = 0, g_destroyed = 0;
struct CTCTrapBase { long TrapType; void ManualReset() { ++g_reset; } };
struct CThing { unsigned char m_state; unsigned char m_flags91; CTCTrapBase trap;
    void SetSuspendAnimation(bool s, long) { g_resume = s; }
    bool GetInterface(CTCTrapBase** out, long type) { if (type == 0x7d) { *out = &trap; return true; } return false; } };
struct CIP { CThing* PItem; CThing* GetPItem() const { return PItem; } ~CIP() { ++g_destroyed; } };
static bool IS_THING_ALIVE(const CIP& p) { if (p.GetPItem() != 0) { if (!(p.GetPItem()->m_flags91 & 1)) return true; } return false; }
struct CIcicleData { CIP PEffect; CIP PTrap; long EffectTimer, TrapTimer; };
struct CIcicleVector {
    CIcicleData* _M_start; CIcicleData* _M_finish;
    CIcicleData* begin() { return _M_start; } CIcicleData* end() { return _M_finish; }
    CIcicleData* erase(CIcicleData* pos)
    {
        for (CIcicleData* p = pos + 1; p != _M_finish; ++p) { (p - 1)->PEffect.PItem = p->PEffect.PItem; (p - 1)->PTrap.PItem = p->PTrap.PItem; }
        --_M_finish; g_destroyed += 2; return pos;
    }
};
struct CMode {
    CThing* ParentThing; CIcicleVector Icicles;
    void BaseOnDeactivate() { ++g_base; }
    void OnDeactivate()
    {
        BaseOnDeactivate();
        if (ParentThing->m_state == 1)
            ParentThing->SetSuspendAnimation(false, 0);
        CIcicleData* icicle = Icicles.begin();
        CTCTrapBase* trap;
        while (icicle != Icicles.end()) {
            if (IS_THING_ALIVE(icicle->PTrap)) {
                icicle->PTrap.GetPItem()->GetInterface(&trap, 0x7d);
                trap->TrapType = 4;
                trap->ManualReset();
            }
            Icicles.erase(icicle);
        }
    }
};
int main()
{
    CThing troll; troll.m_state = 1; CThing a, b, c; a.m_flags91 = 0; b.m_flags91 = 1; c.m_flags91 = 0;
    a.trap.TrapType = b.trap.TrapType = c.trap.TrapType = 0;
    static CIcicleData data[3];
    data[0].PTrap.PItem = &a; data[1].PTrap.PItem = &b; data[2].PTrap.PItem = &c;
    CMode m; m.ParentThing = &troll; m.Icicles._M_start = data; m.Icicles._M_finish = data + 3;
    g_destroyed = 0; m.OnDeactivate();
    if (g_base != 1 || g_resume != 0 || g_reset != 2 || a.trap.TrapType != 4 || b.trap.TrapType != 0 || c.trap.TrapType != 4
        || m.Icicles._M_finish != m.Icicles._M_start || g_destroyed != 6)
    { std::printf("008d9650_TEST FAIL\n"); return 1; }
    std::printf("008d9650_TEST PASS\n");
    return 0;
}