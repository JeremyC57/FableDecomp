// Test: IsPlayerCarryingWeapon -- true only when the creature has the opinion-of-hero TC (0x4F) and it holds a still-happening deed of type 1
#include <cstdio>
struct COpinion { bool hasDeed1Happening; const void* FindMostRecentDeed(long type, bool, long, bool) const { return (type == 1 && hasDeed1Happening) ? this : 0; } };
struct CCreature {
    bool hasTC4F; COpinion opinion;
    bool HasTC(long t) const { if (t == 0x4F && hasTC4F) return true; return false; }
    const COpinion* PeekTC(long) const { return &opinion; }
};
static bool IsPlayerCarryingWeapon(const CCreature* creature)
{
    if (creature->HasTC(0x4F)) {
        const COpinion* opinion = creature->PeekTC(0x4F);
        if (opinion->FindMostRecentDeed(1, true, 0, false)) return true;
    }
    return false;
}
int main()
{
    CCreature a = { true, { true } }, b = { true, { false } }, c = { false, { true } };
    if (!IsPlayerCarryingWeapon(&a) || IsPlayerCarryingWeapon(&b) || IsPlayerCarryingWeapon(&c)) { std::printf("00939e70_TEST FAIL\n"); return 1; }
    std::printf("00939e70_TEST PASS\n");
    return 0;
}