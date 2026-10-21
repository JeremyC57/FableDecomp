// Behaviour test for 00c41e40: src/00c41e40.cpp copied verbatim (shared headers expanded inline), callees stubbed below so calls
// and arguments are observable. CCombatant::SetCombatParameters(const SCombatParameters&)
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// CCombatant::SetCombatParameters(const SCombatParameters&) -- copy each combat parameter that changed into the combatant and raise the matching dirty bits (0x1000000 for most, 0x400000 / 0x1400000 for the third to sixth)
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C41E40, 368 bytes. `ret 4`, no callees.
//
// Shape: every field is copied through a block-scoped local of its own type, `{ T v = p.Field; if (member != v) { member = v; m_dirtyFlags |= bit; } }`.
// The float local is what lets VC7.1 hoist the flag load above the x87 store (`mov eax,[flags] ; fstp member ; or eax,bit`);
// an inline template setter (by value or const&), a float overload or a direct `if (m != p.F)` all store the float first.
// Each float field is loaded once, duplicated and compared with `fucompp ; test ah,44h ; jnp`; the 0x1000000 mask is kept in edi.
// No PDB layout exists for CCombatant (local model, as the landed ComputeSpellPower 0x00C41BB0 / ComputeLightingAdjustment
// 0x00C41A20, whose float at +0x1c4 this body also writes).
// Layout: local model (descriptive member names; the parameter block is 0x28 bytes).
struct SCombatParameters {
    long  Value0;                           // +0x00
    long  Value1;                           // +0x04
    long  Value2;                           // +0x08
    float Value3;                           // +0x0c
    long  Value4;                           // +0x10
    float Value5;                           // +0x14
    long  Value6;                           // +0x18
    float Value7;                           // +0x1c
    float Value8;                           // +0x20
    float Value9;                           // +0x24
};
struct CCombatant {
    unsigned char _pad[0x1a4];
    long          m_param0;                 // +0x1a4
    long          m_param1;                 // +0x1a8
    long          m_param2;                 // +0x1ac
    float         m_param3;                 // +0x1b0
    long          m_param4;                 // +0x1b4
    float         m_param5;                 // +0x1b8
    long          m_param6;                 // +0x1bc
    float         m_param7;                 // +0x1c0
    float         m_spellScale;             // +0x1c4 (read by ComputeSpellPower)
    float         m_param9;                 // +0x1c8
    unsigned char _pad1cc[0xc];             // +0x1cc
    unsigned long m_dirtyFlags;             // +0x1d8
    void SetCombatParameters(const SCombatParameters& p);
};

void CCombatant::SetCombatParameters(const SCombatParameters& p)
{
    { long v = p.Value0; if (m_param0 != v) { m_param0 = v; m_dirtyFlags |= 0x1000000; } }
    { long v = p.Value1; if (m_param1 != v) { m_param1 = v; m_dirtyFlags |= 0x1000000; } }
    { long v = p.Value2; if (m_param2 != v) { m_param2 = v; m_dirtyFlags |= 0x400000; } }
    { float v = p.Value3; if (m_param3 != v) { m_param3 = v; m_dirtyFlags |= 0x400000; } }
    { long v = p.Value4; if (m_param4 != v) { m_param4 = v; m_dirtyFlags |= 0x1400000; } }
    { float v = p.Value5; if (m_param5 != v) { m_param5 = v; m_dirtyFlags |= 0x1400000; } }
    { long v = p.Value6; if (m_param6 != v) { m_param6 = v; m_dirtyFlags |= 0x1000000; } }
    { float v = p.Value7; if (m_param7 != v) { m_param7 = v; m_dirtyFlags |= 0x1000000; } }
    { float v = p.Value8; if (m_spellScale != v) { m_spellScale = v; m_dirtyFlags |= 0x1000000; } }
    { float v = p.Value9; if (m_param9 != v) { m_param9 = v; m_dirtyFlags |= 0x1000000; } }
}

// ---- stubs and driver ----
int main()
{
    static double buf[(sizeof(CCombatant) + 7) / 8];
    CCombatant& c = *(CCombatant*)buf;
    memset(buf, 0, sizeof(buf));
    SCombatParameters p;
    p.Value0 = 0; p.Value1 = 0; p.Value2 = 0; p.Value3 = 0.0f; p.Value4 = 0;
    p.Value5 = 0.0f; p.Value6 = 0; p.Value7 = 0.0f; p.Value8 = 0.0f; p.Value9 = 0.0f;
    c.SetCombatParameters(p);
    CHECK(c.m_dirtyFlags == 0);
    p.Value2 = 5; c.SetCombatParameters(p);
    CHECK(c.m_param2 == 5); CHECK(c.m_dirtyFlags == 0x400000);
    c.m_dirtyFlags = 0; p.Value5 = 2.5f; c.SetCombatParameters(p);
    CHECK(c.m_param5 == 2.5f); CHECK(c.m_dirtyFlags == 0x1400000);
    c.m_dirtyFlags = 0; p.Value8 = 1.25f; p.Value0 = 3; c.SetCombatParameters(p);
    CHECK(c.m_spellScale == 1.25f); CHECK(c.m_param0 == 3); CHECK(c.m_dirtyFlags == 0x1000000);
    c.m_dirtyFlags = 0; c.SetCombatParameters(p);
    CHECK(c.m_dirtyFlags == 0);
    c.m_dirtyFlags = 0x10; p.Value3 = -1.0f; p.Value9 = 4.0f; p.Value4 = 9; p.Value1 = 8; p.Value6 = 7; p.Value7 = 0.5f;
    c.SetCombatParameters(p);
    CHECK(c.m_param3 == -1.0f && c.m_param9 == 4.0f && c.m_param4 == 9 && c.m_param1 == 8 && c.m_param6 == 7 && c.m_param7 == 0.5f);
    CHECK(c.m_dirtyFlags == (0x10 | 0x400000 | 0x1400000 | 0x1000000));
    if (fails == 0) printf("00c41e40_TEST PASS\n"); else printf("00c41e40_TEST FAIL\n");
    return fails;
}
