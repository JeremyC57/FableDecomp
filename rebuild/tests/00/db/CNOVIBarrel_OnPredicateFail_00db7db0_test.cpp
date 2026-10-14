#include <cstdio>
#include <cstdlib>
#include <cstring>
struct C3DVector { float X, Y, Z; };
#pragma pack(push, 1)
struct SQuest { unsigned char _pad[0x74]; bool m_74, m_75; C3DVector m_pos; };
#pragma pack(pop)
struct CScriptThing { virtual void v0() { } virtual void v1() { } virtual void v2() { } virtual void v3() { } virtual void v4() { } virtual void v5() { } virtual C3DVector* GetPosition() { return &m_pos; } C3DVector m_pos; };
struct CNOVI_Barrel { long m_0, m_4; CScriptThing Thing; SQuest* PQuest; void OnPredicateFail() { PQuest->m_74 = true; PQuest->m_75 = true; PQuest->m_pos = *Thing.GetPosition(); } };

int main() {
    CNOVI_Barrel b; SQuest q; std::memset(&q, 0, sizeof q); b.PQuest = &q; b.Thing.m_pos.X = 1; b.Thing.m_pos.Y = 2; b.Thing.m_pos.Z = 3; b.OnPredicateFail(); if (!q.m_74 || !q.m_75 || q.m_pos.X != 1 || q.m_pos.Y != 2 || q.m_pos.Z != 3) { std::printf("00db7db0_TEST FAIL\n"); return 1; }
    std::printf("00db7db0_TEST PASS\n"); return 0;
}