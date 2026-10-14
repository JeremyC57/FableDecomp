#include <cstdio>
static int g_ptrAssign = 0;
struct CIntelligentPointer_00c48770 { void* m_pItem; void* m_pLink; CIntelligentPointer_00c48770& operator=(const CIntelligentPointer_00c48770& o) { m_pItem = o.m_pItem; ++g_ptrAssign; return *this; } };
struct STriple_00c48770 { long m_x, m_y, m_z; };
struct SCreatureRecord_00c48770 { long m_a; STriple_00c48770 m_position; CIntelligentPointer_00c48770 m_creature; long m_b;
    SCreatureRecord_00c48770& operator=(const SCreatureRecord_00c48770& o) { m_a = o.m_a; m_position = o.m_position; m_creature = o.m_creature; m_b = o.m_b; return *this; } };
int main() {
    SCreatureRecord_00c48770 a, b; a.m_a = 1; a.m_position.m_x = 2; a.m_position.m_y = 3; a.m_position.m_z = 4; a.m_creature.m_pItem = &a; a.m_creature.m_pLink = 0; a.m_b = 5; b.m_creature.m_pItem = 0;
    SCreatureRecord_00c48770& r = (b = a);
    if (&r != &b || b.m_a != 1 || b.m_position.m_x != 2 || b.m_position.m_z != 4 || b.m_creature.m_pItem != &a || b.m_b != 5 || g_ptrAssign != 1) { std::printf("00c48770_TEST FAIL\n"); return 1; }
    std::printf("00c48770_TEST PASS\n"); return 0;
}