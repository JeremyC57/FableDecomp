// Behaviour test for 0x006F46D0 CDefendingCombatantInfo copy constructor: containers copied, scalars copied, the
// intelligent pointer re-pointed at the same attacker, the def pointer shared with its count bumped.
#include <cstdio>
static int g_mapCopies = 0, g_vecCopies = 0;
struct CAttackerMap { int n; CAttackerMap() : n(3) {} CAttackerMap(const CAttackerMap& o) : n(o.n) { ++g_mapCopies; } };
struct CCombatRingVector { int n; CCombatRingVector() : n(2) {} CCombatRingVector(const CCombatRingVector& o) : n(o.n) { ++g_vecCopies; } };
struct CCombatWheel { CCombatRingVector CombatRings; };
struct C3DVector { unsigned int X, Y, Z; C3DVector() : X(1), Y(2), Z(3) {} C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) {} };
struct CIntelligentPointer { void* m_p; CIntelligentPointer() : m_p(0) {} CIntelligentPointer(const CIntelligentPointer& o) : m_p(0) { m_p = o.m_p; } };
struct CDefPointee { int m_refCount; };
struct CDefPointer { CDefPointee* Object; CDefPointer() : Object(0) {} CDefPointer(const CDefPointer& o) : Object(o.Object) { if (Object) ++Object->m_refCount; } };
struct CInfo {
    CAttackerMap Attackers; CCombatWheel CombatWheel; C3DVector Pos; int Frame; CIntelligentPointer PLastAttacker; CDefPointer PAttackPattern; int Index, Next;
    CInfo() : Frame(9), Index(4), Next(5) {}
    CInfo(const CInfo& o) : Attackers(o.Attackers), CombatWheel(o.CombatWheel), Pos(o.Pos), Frame(o.Frame), PLastAttacker(o.PLastAttacker),
        PAttackPattern(o.PAttackPattern), Index(o.Index), Next(o.Next) {}
};
int main()
{
    int attacker = 0; CDefPointee def; def.m_refCount = 1;
    CInfo a; a.PLastAttacker.m_p = &attacker; a.PAttackPattern.Object = &def;
    CInfo b(a);
    if (g_mapCopies != 1 || g_vecCopies != 1 || b.Pos.Z != 3 || b.Frame != 9 || b.PLastAttacker.m_p != &attacker || b.PAttackPattern.Object != &def
        || def.m_refCount != 2 || b.Index != 4 || b.Next != 5) { std::printf("006f46d0_TEST FAIL\n"); return 1; }
    std::printf("006f46d0_TEST PASS\n");
    return 0;
}