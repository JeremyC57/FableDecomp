// behaviour test for 0x00847990 CCreatureAction_LearnExpression::PerformAction(): a live teacher with a CTCExpression makes
// the player creature learn the expression def's instantiation name (-1, false); dead / missing / component-less teachers do nothing
#include <cstdio>
#include <cstring>
struct Def { const char* name; };
struct Expression { const Def* def; const Def* GetPDef() const { return def; } };
struct Thing {
    bool dead; const Expression* expr;
    bool IsDead() const { return dead; }
    bool PeekTC(const Expression** out) const { if (!expr) return false; *out = expr; return true; }
};
struct Player {
    char learned[32]; long a; bool b; int count;
    bool LearnExpression(const char* name, long x, bool y) { std::strcpy(learned, name); a = x; b = y; ++count; return true; }
};
struct Action {
    Thing* PLearnFromThing; Player* PPlayerCreature;
    void PerformAction()
    {
        const Expression* expression;
        if (PLearnFromThing && !PLearnFromThing->IsDead() && PLearnFromThing->PeekTC(&expression)) {
            const Def* def = expression->GetPDef();
            PPlayerCreature->LearnExpression(def->name, -1, false);
        }
    }
};
int main()
{
    int fails = 0;
    Def def = { "EXPRESSION_HERO_WAVE" };
    Expression ex = { &def };
    Thing teacher = { false, &ex };
    Player p; p.count = 0;
    Action a = { &teacher, &p };
    a.PerformAction();
    if (p.count != 1 || std::strcmp(p.learned, "EXPRESSION_HERO_WAVE") != 0 || p.a != -1 || p.b) fails |= 1;
    teacher.dead = true; a.PerformAction();
    Thing plain = { false, 0 }; a.PLearnFromThing = &plain; a.PerformAction();
    a.PLearnFromThing = 0; a.PerformAction();
    if (p.count != 1) fails |= 2;
    if (fails) { std::printf("00847990_TEST FAIL %d\n", fails); return 1; }
    std::printf("00847990_TEST PASS\n");
    return 0;
}