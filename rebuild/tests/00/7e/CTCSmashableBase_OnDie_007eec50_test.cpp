#include <cstdio>

struct CSmashableDef { bool Smashable; };
struct CTCSmashableBase {
    CSmashableDef* def; bool Smashed; int smashes;
    CSmashableDef* GetDef() { return def; }
    virtual void Smash() { ++smashes; Smashed = true; }
    virtual ~CTCSmashableBase() {}
    void OnDie()
    {
        bool smashable = GetDef()->Smashable;
        if (smashable && !Smashed)
            Smash();
    }
};
static bool Run()
{
    CSmashableDef d; d.Smashable = false;
    CTCSmashableBase s; s.def = &d; s.Smashed = false; s.smashes = 0;
    s.OnDie(); bool ok = s.smashes == 0;
    d.Smashable = true; s.OnDie(); ok = ok && s.smashes == 1;
    s.OnDie(); return ok && s.smashes == 1;
}

int main()
{
    if (!Run()) { std::printf("007eec50_TEST FAIL\n"); return 1; }
    std::printf("007eec50_TEST PASS\n");
    return 0;
}