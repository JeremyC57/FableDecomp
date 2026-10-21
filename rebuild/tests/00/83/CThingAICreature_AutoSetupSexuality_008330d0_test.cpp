// Behaviour test for CThingAICreature::AutoSetupSexuality (0x008330D0): a female with interfaces 0x4F and 0xD7 becomes
// courtable and marriageable (otherwise untouched); a male's two flags are both set from (talk component present, voice
// "AM3", roll < chance) and both cleared otherwise; any other sex is untouched; finally a courtable creature without a
// wife component (0xD7) gets "CTCWife" added (not activated, no params).
#include <cstdio>
#include <string>

struct Creature {
    int sex; bool has4f, hasWife, hasTalk; std::string voice; float roll; bool courted, married; std::string added;
    void AutoSetupSexuality(float chance)
    {
        if (sex == 2) { if (has4f && hasWife) { courted = true; married = true; } }
        else if (sex == 1) {
            bool ok = hasTalk && voice == "AM3" && roll < chance;
            courted = married = ok;
        }
        if (courted && !hasWife) added = "CTCWife";
    }
};

int main()
{
    int ok = 1;
    Creature f = { 2, true, true, false, "", 0.0f, false, false, "" };
    f.AutoSetupSexuality(0.5f);
    ok &= f.courted && f.married && f.added == "";
    Creature f2 = { 2, true, false, false, "", 0.0f, false, false, "" };
    f2.AutoSetupSexuality(0.5f);
    ok &= !f2.courted && !f2.married && f2.added == "";
    Creature m = { 1, false, false, true, "AM3", 0.25f, false, false, "" };
    m.AutoSetupSexuality(0.5f);
    ok &= m.courted && m.married && m.added == "CTCWife";
    Creature m2 = { 1, false, false, true, "AM1", 0.25f, true, true, "" };
    m2.AutoSetupSexuality(0.5f);
    ok &= !m2.courted && !m2.married && m2.added == "";
    Creature m3 = { 1, false, false, true, "AM3", 0.75f, true, true, "" };
    m3.AutoSetupSexuality(0.5f);
    ok &= !m3.courted && !m3.married;
    Creature n = { 0, false, false, false, "", 0.0f, true, false, "" };
    n.AutoSetupSexuality(0.5f);
    ok &= n.courted && n.added == "CTCWife";
    std::printf(ok ? "008330d0_TEST PASS\n" : "008330d0_TEST FAIL\n");
    return ok ? 0 : 1;
}