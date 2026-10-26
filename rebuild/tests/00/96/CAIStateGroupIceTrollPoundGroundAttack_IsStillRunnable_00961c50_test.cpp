#include <cstdio>
// Behaviour model of CAIStateGroup_IceTrollPoundGroundAttack::IsStillRunnable() const @ 0x00961C50 (private const; the
// range predicate takes const references, as Ego_r.pdb names them).
int main();
class CThing { public: unsigned char _pad[0x91]; unsigned char m_flags91; bool IsDead() const { return (m_flags91 & 1) != 0; } };
struct CBaseIntelligentPointer { virtual void v00() {} CThing* m_pItem; CThing* GetPItem() const { return m_pItem; } };
class CAIBrain { public: unsigned char _pad[0x18]; CThing* PCreature; };
static bool g_inRange = true; static const CThing* g_rangeCreature = 0; static const CThing* g_rangeTarget = 0;
static bool IsThingInRangeOfPound(const CThing& c, const CThing& t) { g_rangeCreature = &c; g_rangeTarget = &t; return g_inRange; }
class CAIStateGroupBase { public: virtual void v00() {} CAIBrain* PBrain; };
class CAIStateGroup_IceTrollPoundGroundAttack : public CAIStateGroupBase {
public:
    CBaseIntelligentPointer PTarget;
private:
    friend int ::main();
    bool IsStillRunnable() const
    {
        if (PTarget.GetPItem() != 0) {
            if (!PTarget.GetPItem()->IsDead()) {
                const CThing& c = *PBrain->PCreature;
                if (IsThingInRangeOfPound(c, *PTarget.GetPItem())) return true;
            }
        }
        return false;
    }
};
int main()
{
    CThing creature, target; target.m_flags91 = 0; CAIBrain brain; brain.PCreature = &creature;
    CAIStateGroup_IceTrollPoundGroundAttack g; g.PBrain = &brain; g.PTarget.m_pItem = &target;
    bool ok = g.IsStillRunnable() == true && g_rangeCreature == &creature && g_rangeTarget == &target;
    g_inRange = false; ok = ok && g.IsStillRunnable() == false;
    g_inRange = true; target.m_flags91 = 1; g_rangeCreature = 0; ok = ok && g.IsStillRunnable() == false && g_rangeCreature == 0;
    target.m_flags91 = 0; g.PTarget.m_pItem = 0; ok = ok && g.IsStillRunnable() == false;
    if (!ok) { std::printf("00961c50_TEST FAIL\n"); return 1; }
    std::printf("00961c50_TEST PASS\n");
    return 0;
}
