#include <cstdio>
static long g_askedAnim = -1;
struct CThingCreatureBase { long combo; bool IsAnimCombinationAnim(long a) { g_askedAnim = a; return a == combo; } };
struct CBaseIntelligentPointer { virtual void v00() {} CThingCreatureBase* m_pItem; CThingCreatureBase* GetPItem() const { return m_pItem; } };
struct CCreatureActionBase {
    virtual void v00() {} virtual void v01() {} virtual void v02() {} virtual void v03() {} virtual void v04() {} virtual void v05() {} virtual void v06() {}
    virtual long GetAnimationIndex() const { return m_anim; }
    unsigned char _pad04[4]; CBaseIntelligentPointer PCreature; long m_anim;
    bool IsPreventingNormalMovement() const { CThingCreatureBase* creature = PCreature.GetPItem(); return !creature->IsAnimCombinationAnim(GetAnimationIndex()); }
};
int main()
{
    CThingCreatureBase c; c.combo = 7; CCreatureActionBase a; a.PCreature.m_pItem = &c; a.m_anim = 7;
    bool r1 = a.IsPreventingNormalMovement(); a.m_anim = 8; bool r2 = a.IsPreventingNormalMovement();
    if (r1 != false || r2 != true || g_askedAnim != 8) { std::printf("00692a20_TEST FAIL\n"); return 1; }
    std::printf("00692a20_TEST PASS\n");
    return 0;
}
