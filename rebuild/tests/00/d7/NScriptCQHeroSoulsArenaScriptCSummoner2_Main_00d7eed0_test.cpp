// Behaviour test for the Hero Souls summoner Main (00d7eed0): wait for a qualifying hit, then set PlayerHitSummoner2.
#include <cstdio>
#include <cstring>
struct Script {
    int frames; int doneAfter; int hitAt; int anySpecialAt; bool specialIs14; bool flag; int filters;
    bool IsDone() { return frames >= doneAfter; }
    void NewScriptFrame() { ++frames; }
    bool MsgIsHitBy(const char* who) { return std::strcmp(who, "SCRIPT_NAME_HERO") == 0 && frames == hitAt; }
    bool MsgIsHitByAnySpecialAbilityFrom(const char*) { return frames == anySpecialAt; }
    bool MsgIsHitBySpecialAbilityFrom(long ability, const char*) { return ability == 14 && specialIs14; }
    void Main()
    {
        ++filters;
        NewScriptFrame();
        if (IsDone()) return;
        while (!(MsgIsHitBy("SCRIPT_NAME_HERO")
                 || (MsgIsHitByAnySpecialAbilityFrom("SCRIPT_NAME_HERO") && !MsgIsHitBySpecialAbilityFrom(14, "SCRIPT_NAME_HERO")))) {
            NewScriptFrame();
            if (IsDone()) return;
        }
        if (IsDone()) return;
        flag = true;   // ParentClass->PlayerHitSummoner2
    }
};
int main()
{
    Script a = { 0, 100, 5, -1, false, false, 0 }; a.Main();      // plain hit at frame 5
    Script b = { 0, 100, -1, 7, true, false, 0 }; b.Main();       // only ability-14 hits: never counts, script ends
    Script c = { 0, 100, -1, 3, false, false, 0 }; c.Main();      // another special ability counts
    Script d = { 0, 4, 5, -1, false, false, 0 }; d.Main();        // done before the hit
    if (!a.flag || a.frames != 5 || b.flag || b.frames != 100 || !c.flag || c.frames != 3 || d.flag || a.filters != 1)
    { std::printf("00d7eed0_TEST FAIL\n"); return 1; }
    std::printf("00d7eed0_TEST PASS\n");
    return 0;
}