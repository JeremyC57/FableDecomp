// Behaviour test for 0x0091F0D0: Update casts the shield spell with the fixed arguments, then terminates.
#include <cstdio>
struct CCreature { int id; };
static int g_order = 0, g_actionDtor = 0, g_nameDtor = 0;
struct CName { const char* text; CName(const char* t) : text(t) {} ~CName() { g_nameDtor = ++g_order; } };
struct CAction { CCreature* caster; long level; bool force; const char* sound; long repeat; float duration;
    CAction(CCreature* c, long l, bool f, const CName& s, long r, float d) : caster(c), level(l), force(f), sound(s.text), repeat(r), duration(d) {}
    ~CAction() { g_actionDtor = ++g_order; } };
struct CGroup { CCreature* creature; bool Terminated; CAction* seen; long seenLevel; bool seenWait;
    long Perform(CAction* a, bool wait) { seen = a; seenLevel = a->level; seenWait = wait; return 2; }
    void Update()
    {
        {
            CName soundName("");
            CCreature* c = creature;
            CAction action(c, 2, true, soundName, 5, 0.0f);
            Perform(&action, false);
        }
        Terminated = true;
    }
};
int main()
{
    CCreature cr = { 1 };
    CGroup g = { &cr, false, 0, 0, true };
    g.Update();
    if (!g.Terminated || g.seenLevel != 2 || g.seenWait || g_actionDtor != 1 || g_nameDtor != 2) {
        std::printf("0091f0d0_TEST FAIL\n"); return 1;
    }
    std::printf("0091f0d0_TEST PASS\n");
    return 0;
}
