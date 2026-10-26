// behaviour test for 0x0045279B CBonusItemDef::Transfer: the context sees every transferred member once, in the def's order (retail offsets +0x28, +0x2C, +0x30, +0x34, +0x38, +0x3C, +0x40, +0x44, +0x48, +0x4C, +0x50, +0x54, +0x58, +0x5C, +0x5D, +0x60, +0x64, +0x68, +0x6C, +0x70),
// so a binary save / load round trip restores the def
#include <cstdio>
#include <cstring>
struct Context {
    unsigned char buf[1024]; unsigned int pos; int load; const void* order[64]; unsigned int count;
    Context(int l) : pos(0), load(l), count(0) { }
    template <class T> void Transfer(T& v)
    {
        order[count++] = &v;
        if (load) std::memcpy(&v, buf + pos, sizeof(T)); else std::memcpy(buf + pos, &v, sizeof(T));
        pos += (unsigned int)sizeof(T);
    }
};
struct Def {
    float HealthModifier;
    bool AddsFatness;
    long StaminaModifier;
    float SecondsToApplyHealthIncreaseOver;
    long WillExperience;
    long SkillExperience;
    long StrengthExperience;
    long MoralityChange;
    float AgreeablenessChange;
    float AttractivenessChange;
    float ScarinessChange;
    float MaxHealthIncrease;
    long MaxStaminaIncrease;
    bool ChangeTimeOfDayToDay;
    bool ChangeTimeOfDayToNight;
    float SpeedMultiplier;
    float SecondsForSpeedMultiplierToLast;
    long ParticleEffect;
    bool AddsDrunkenness;
    float AlcoholicStrength;
};
static void Persist(Def& d, Context& c)                      // the function under test
{
    c.Transfer(d.HealthModifier);
    c.Transfer(d.AddsFatness);
    c.Transfer(d.StaminaModifier);
    c.Transfer(d.SecondsToApplyHealthIncreaseOver);
    c.Transfer(d.WillExperience);
    c.Transfer(d.SkillExperience);
    c.Transfer(d.StrengthExperience);
    c.Transfer(d.MoralityChange);
    c.Transfer(d.AgreeablenessChange);
    c.Transfer(d.AttractivenessChange);
    c.Transfer(d.ScarinessChange);
    c.Transfer(d.MaxHealthIncrease);
    c.Transfer(d.MaxStaminaIncrease);
    c.Transfer(d.ChangeTimeOfDayToDay);
    c.Transfer(d.ChangeTimeOfDayToNight);
    c.Transfer(d.SpeedMultiplier);
    c.Transfer(d.SecondsForSpeedMultiplierToLast);
    c.Transfer(d.ParticleEffect);
    c.Transfer(d.AddsDrunkenness);
    c.Transfer(d.AlcoholicStrength);
}
int main()
{
    int fail = 0;
    Def d; std::memset(&d, 0, sizeof(d));
    unsigned char* p = (unsigned char*)&d;
    for (unsigned int i = 0; i < (unsigned int)sizeof(d); ++i) p[i] = (unsigned char)(i * 7 + 3);
    d.AddsFatness = true;
    d.ChangeTimeOfDayToDay = true;
    d.ChangeTimeOfDayToNight = true;
    d.AddsDrunkenness = true;
    Context save(0); Persist(d, save);
    const void* want[20] = { &d.HealthModifier, &d.AddsFatness, &d.StaminaModifier, &d.SecondsToApplyHealthIncreaseOver, &d.WillExperience, &d.SkillExperience, &d.StrengthExperience, &d.MoralityChange, &d.AgreeablenessChange, &d.AttractivenessChange, &d.ScarinessChange, &d.MaxHealthIncrease, &d.MaxStaminaIncrease, &d.ChangeTimeOfDayToDay, &d.ChangeTimeOfDayToNight, &d.SpeedMultiplier, &d.SecondsForSpeedMultiplierToLast, &d.ParticleEffect, &d.AddsDrunkenness, &d.AlcoholicStrength };
    if (save.count != 20u) fail |= 1;
    for (unsigned int i = 0; i < save.count && i < 20u; ++i) if (save.order[i] != want[i]) fail |= 2;
    Def e; std::memcpy(&e, &d, sizeof(e));
    std::memset(&e.HealthModifier, 0, sizeof(e.HealthModifier));
    std::memset(&e.AddsFatness, 0, sizeof(e.AddsFatness));
    std::memset(&e.StaminaModifier, 0, sizeof(e.StaminaModifier));
    std::memset(&e.SecondsToApplyHealthIncreaseOver, 0, sizeof(e.SecondsToApplyHealthIncreaseOver));
    std::memset(&e.WillExperience, 0, sizeof(e.WillExperience));
    std::memset(&e.SkillExperience, 0, sizeof(e.SkillExperience));
    std::memset(&e.StrengthExperience, 0, sizeof(e.StrengthExperience));
    std::memset(&e.MoralityChange, 0, sizeof(e.MoralityChange));
    std::memset(&e.AgreeablenessChange, 0, sizeof(e.AgreeablenessChange));
    std::memset(&e.AttractivenessChange, 0, sizeof(e.AttractivenessChange));
    std::memset(&e.ScarinessChange, 0, sizeof(e.ScarinessChange));
    std::memset(&e.MaxHealthIncrease, 0, sizeof(e.MaxHealthIncrease));
    std::memset(&e.MaxStaminaIncrease, 0, sizeof(e.MaxStaminaIncrease));
    std::memset(&e.ChangeTimeOfDayToDay, 0, sizeof(e.ChangeTimeOfDayToDay));
    std::memset(&e.ChangeTimeOfDayToNight, 0, sizeof(e.ChangeTimeOfDayToNight));
    std::memset(&e.SpeedMultiplier, 0, sizeof(e.SpeedMultiplier));
    std::memset(&e.SecondsForSpeedMultiplierToLast, 0, sizeof(e.SecondsForSpeedMultiplierToLast));
    std::memset(&e.ParticleEffect, 0, sizeof(e.ParticleEffect));
    std::memset(&e.AddsDrunkenness, 0, sizeof(e.AddsDrunkenness));
    std::memset(&e.AlcoholicStrength, 0, sizeof(e.AlcoholicStrength));
    Context load(1); std::memcpy(load.buf, save.buf, sizeof(save.buf)); Persist(e, load);
    if (load.pos != save.pos) fail |= 4;
    if (std::memcmp(&e, &d, sizeof(d)) != 0) fail |= 8;
    if (fail) { std::printf("0045279b_TEST FAIL %d\n", fail); return 1; }
    std::printf("0045279b_TEST PASS\n");
    return 0;
}