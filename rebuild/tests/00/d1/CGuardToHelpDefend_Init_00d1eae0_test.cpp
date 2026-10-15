#include <cstdio>
#include <cstring>
// Host model of CGuardToHelpDefend::Init: clear the state, attach the script's thing to the named village, clear the flag.
struct Thing { const char* name; };
struct Interface {
    Thing village; const Thing* attachedTo; const Thing* attached; bool flag; int flagCalls;
    Thing GetThingWithScriptName(const char* n) const { Thing t = village; if (std::strcmp(n, village.name) != 0) t.name = 0; return t; }
    void Attach(const Thing& me, const Thing& v) { attached = &me; attachedTo = v.name ? &village : 0; }
    void SetFlag(const Thing&, bool f) { flag = f; ++flagCalls; }
};
struct Script {
    Interface* itf; Thing me; bool guarding; long state;
    void Init()
    {
        guarding = false; state = 0;
        { const char* name = "VILLAGE_KNOTHOLEGLADE"; itf->Attach(me, itf->GetThingWithScriptName(name)); }
        itf->SetFlag(me, false);
    }
};
int main()
{
    Interface i; i.village.name = "VILLAGE_KNOTHOLEGLADE"; i.attachedTo = 0; i.attached = 0; i.flag = true; i.flagCalls = 0;
    Script s; s.itf = &i; s.me.name = "GUARD"; s.guarding = true; s.state = 5;
    s.Init();
    if (s.guarding || s.state != 0 || i.attached != &s.me || i.attachedTo != &i.village || i.flag || i.flagCalls != 1) { std::printf("00d1eae0_TEST FAIL\n"); return 1; }
    std::printf("00d1eae0_TEST PASS\n");
    return 0;
}