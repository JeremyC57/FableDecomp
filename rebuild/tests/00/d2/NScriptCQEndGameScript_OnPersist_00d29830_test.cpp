// behaviour test for 0x00D29830 CQ_EndGameScript::OnPersist: UndeadBar then QuestStartScreened are transferred under their own names with zero defaults, so a reset context restores both defaults and a save/load round trip keeps the values
#include <cstdio>
#include <cstring>
#include <map>
#include <string>
struct Context {
    int mode;                                                // 0 save, 1 load, 2 reset
    std::map<std::string, double> store; std::string order;
    template <class T> void Transfer(const char* name, T& value, const T& def)
    {
        order += name; order += ";";
        if (mode == 0) store[name] = (double)value;
        else if (mode == 1) value = (T)store[name];
        else value = def;
    }
};
struct Script { float m1; bool m2; };
static void OnPersist(Script& s, Context& c)                  // the function under test
{
    c.Transfer("UndeadBar", s.m1, (float)(0.0f));
    c.Transfer("QuestStartScreened", s.m2, (bool)(false));
}
int main()
{
    int fail = 0;
    Script s; s.m1 = (float)1; s.m2 = (bool)1;
    Context save; save.mode = 0; OnPersist(s, save);
    if (save.order != std::string("UndeadBar;QuestStartScreened;")) fail |= 1;
    Script r; r.m1 = (float)0; r.m2 = (bool)0;
    Context load; load.mode = 1; load.store = save.store; OnPersist(r, load);
    if (r.m1 != s.m1 || r.m2 != s.m2) fail |= 2;
    Context reset; reset.mode = 2; OnPersist(r, reset);
    if (r.m1 != (float)(0.0f) || r.m2 != (bool)(false)) fail |= 4;
    if (fail) { std::printf("00d29830_TEST FAIL %d\n", fail); return 1; }
    std::printf("00d29830_TEST PASS\n");
    return 0;
}