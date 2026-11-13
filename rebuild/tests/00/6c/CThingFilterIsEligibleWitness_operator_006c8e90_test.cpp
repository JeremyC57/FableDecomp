// Behaviour test for CThingFilter_IsEligibleWitness::operator() (0x006C8E90): a model of the witness decision (type, sleeping,
// sub-filters, faction, perceiving, unknown-threat inversion with awareness / sight-radius + nav line of sight), spelled with the
// row's control structure (&& chain, faction ternary, nested single-exit blocks).
#include <cstdio>
struct Thing {
    int Type; bool HasModeTC; int Mode; bool HasOpinions; bool DeedEnabled; bool IsRecipient;
    bool HasEnemyTC; bool Allied; bool Enemy;
    bool AwareOfPerp; bool HasPerceiveTC; bool PerceivesPerp; bool Heard; bool LOS; bool Smelt; bool IgnoreFOVLOS;
    bool AwareOfTarget; bool HasNavTC; float X, Y, SightRadius; bool NavLOS;
};
struct Deed { bool TargetAlive; float X, Y; };
struct Filter {
    const Deed* PDeed; bool IncludeHearing;
    bool UnknownThreatSearch, UseHearing, UsePerceiving, UseIsNot, UseFaction, ReverseFactionSearch;
    bool IsPerceivingPerpetrator(const Thing& t) const
    {
        bool result = false;
        if (t.PerceivesPerp) result = (IncludeHearing && t.Heard) || t.LOS || t.Smelt;
        return result;
    }
    bool IsAllied(const Thing& t) const { return t.HasEnemyTC ? t.Allied : false; }
    bool IsEnemy(const Thing& t) const { return t.HasEnemyTC ? t.Enemy : false; }
    bool operator()(const Thing& thing) const
    {
        if (thing.Type == 1) {
            bool result = !(thing.HasModeTC && thing.Mode == 13) && thing.HasOpinions && thing.DeedEnabled
                          && (!UseIsNot || !thing.IsRecipient);
            if (result && UseFaction)
                result = ReverseFactionSearch ? IsEnemy(thing) : IsAllied(thing);
            if (!result || !UsePerceiving)
                return result;
            bool ret;
            do {
                bool perceiving = UseHearing ? thing.AwareOfPerp : IsPerceivingPerpetrator(thing);
                result = perceiving && thing.HasPerceiveTC && thing.PerceivesPerp && thing.IgnoreFOVLOS;
                if (!UnknownThreatSearch) { ret = result; break; }
                result = !result;
                if (!result) { ret = result; break; }
                bool aware;
                do {
                    if (PDeed && PDeed->TargetAlive && thing.AwareOfTarget) { aware = true; break; }
                    if (PDeed && thing.HasPerceiveTC) {
                        if (thing.HasNavTC) {
                            float radius = thing.SightRadius;
                            float dx = thing.X - PDeed->X, dy = thing.Y - PDeed->Y;
                            if (dy * dy + dx * dx < radius * radius && thing.NavLOS) { aware = true; break; }
                        }
                    }
                    { aware = false; break; }
                } while (0);
                ret = aware;
            } while (0);
            return ret;
        }
        return false;
    }
};
static int fails = 0;
static void check(bool got, bool want, int line) { if (got != want) { std::printf("fail at line %d\n", line); ++fails; } }
int main()
{
    Deed deed = { true, 0.0f, 0.0f };
    Thing base = { 1, true, 2, true, true, false, true, true, false, false, true, true, false, true, false, true,
                   false, true, 3.0f, 4.0f, 10.0f, true };
    Filter f = { &deed, true, false, false, false, false, false, false };
    Thing t = base;
    check(f(t), true, __LINE__);                       // plain creature with opinions
    t.Type = 2; check(f(t), false, __LINE__); t = base;
    t.Mode = 13; check(f(t), false, __LINE__); t = base;  // asleep in bed
    t.HasModeTC = false; t.Mode = 13; check(f(t), true, __LINE__); t = base;
    t.HasOpinions = false; check(f(t), false, __LINE__); t = base;
    t.DeedEnabled = false; check(f(t), false, __LINE__); t = base;
    f.UseIsNot = true; t.IsRecipient = true; check(f(t), false, __LINE__); t.IsRecipient = false; check(f(t), true, __LINE__);
    f.UseFaction = true; check(f(t), true, __LINE__); t.Allied = false; check(f(t), false, __LINE__);
    f.ReverseFactionSearch = true; t.Enemy = true; check(f(t), true, __LINE__); t.HasEnemyTC = false; check(f(t), false, __LINE__);
    t = base; f.UseFaction = false; f.UsePerceiving = true;
    check(f(t), true, __LINE__);                       // perceives via LOS, ignore-FOV LOS
    t.IgnoreFOVLOS = false; check(f(t), false, __LINE__); t = base;
    t.LOS = false; check(f(t), false, __LINE__); t.Heard = true; check(f(t), true, __LINE__); t = base;
    f.UseHearing = true; check(f(t), false, __LINE__); t.AwareOfPerp = true; check(f(t), true, __LINE__);
    f.UseHearing = false; t = base;
    f.UnknownThreatSearch = true;
    check(f(t), false, __LINE__);                      // sees the perpetrator: not an unknown-threat witness
    t.IgnoreFOVLOS = false;                            // does not see: needs awareness of the target or sight of the deed
    check(f(t), true, __LINE__);                       // 3-4-5 triangle inside radius 10, nav LOS
    t.NavLOS = false; check(f(t), false, __LINE__);
    t.AwareOfTarget = true; check(f(t), true, __LINE__);
    deed.TargetAlive = false; check(f(t), false, __LINE__);
    t.NavLOS = true; t.SightRadius = 5.0f; check(f(t), false, __LINE__);   // distance 5 is not < 5
    t.SightRadius = 5.5f; check(f(t), true, __LINE__);
    t.HasNavTC = false; check(f(t), false, __LINE__);
    f.PDeed = 0; t = base; t.IgnoreFOVLOS = false; check(f(t), false, __LINE__);
    if (fails) { std::printf("006c8e90_TEST FAIL\n"); return 1; }
    std::printf("006c8e90_TEST PASS\n");
    return 0;
}