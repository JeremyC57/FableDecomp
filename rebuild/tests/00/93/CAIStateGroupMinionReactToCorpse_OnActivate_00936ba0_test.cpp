#include <cstdio>
// CAIStateGroup_MinionReactToCorpse::OnActivate: unless life state 3, AddMode(15)
struct Modes { unsigned long last; int calls; void AddMode(unsigned long m) { last = m; ++calls; } };
struct Group { long lifeState; Modes* modes; void OnActivate() { if (lifeState != 3) modes->AddMode(15); } };
int main()
{
    Modes m; m.last = 0; m.calls = 0; Group g; g.lifeState = 1; g.modes = &m;
    g.OnActivate(); if (m.calls != 1 || m.last != 15) { std::printf("00936ba0_TEST FAIL\n"); return 1; }
    g.lifeState = 3; g.OnActivate(); if (m.calls != 1) { std::printf("00936ba0_TEST FAIL\n"); return 1; }
    std::printf("00936ba0_TEST PASS\n"); return 0;
}