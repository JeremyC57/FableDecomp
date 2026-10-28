// Test: CAIStateGroup_ScorpionKing ctor -- base ctor, two empty intelligent pointers and an empty vector
#include <cstdio>
struct CIntelligentPointer { void* PData; CIntelligentPointer() : PData(0) { } };
struct CVec { void* s; void* f; void* e; CVec() : s(0), f(0), e(0) { } };
struct CBase { int baseBuilt; CBase() : baseBuilt(1) { } };
struct CAIStateGroup_ScorpionKing : CBase {
    CIntelligentPointer PArenaCentre; CVec TrapThings; CIntelligentPointer PEnemy;
    CAIStateGroup_ScorpionKing() { }
};
int main()
{
    CAIStateGroup_ScorpionKing k;
    if (!k.baseBuilt || k.PArenaCentre.PData || k.PEnemy.PData || k.TrapThings.s || k.TrapThings.f || k.TrapThings.e) { std::printf("00903177_TEST FAIL\n"); return 1; }
    std::printf("00903177_TEST PASS\n");
    return 0;
}