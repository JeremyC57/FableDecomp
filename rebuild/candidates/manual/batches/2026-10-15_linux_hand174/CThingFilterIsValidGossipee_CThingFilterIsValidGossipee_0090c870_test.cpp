#include <cstdio>
struct Faction { int id; };
struct Village { int id; };
struct FM { Faction* f; };
struct VM { Village* v; Village* GetPTCVillage() const { return v; } };
struct Thing { FM* fm; VM* vm;
    bool GetFM(FM** o) { if (fm) { *o = fm; return true; } return false; }
    bool GetVM(VM** o) { if (vm) { *o = vm; return true; } return false; } };
struct F { Thing* T; Faction* PFaction; Village* PTCVillage;
    F(Thing* t) : T(t), PFaction(0), PTCVillage(0) {
        FM* f; if (t->GetFM(&f)) PFaction = f->f;
        VM* m; if (t->GetVM(&m)) PTCVillage = m->GetPTCVillage(); } };
int main()
{
    Faction fa = { 1 }; Village vi = { 2 };
    FM fm = { &fa }; VM vm = { &vi };
    Thing a = { &fm, &vm }, b = { 0, &vm }, c = { &fm, 0 };
    F fa1(&a), fb(&b), fc(&c);
    bool ok = fa1.T == &a && fa1.PFaction == &fa && fa1.PTCVillage == &vi
        && fb.PFaction == 0 && fb.PTCVillage == &vi && fc.PFaction == &fa && fc.PTCVillage == 0;
    if (!ok) { std::printf("0090c870_TEST FAIL\n"); return 1; }
    std::printf("0090c870_TEST PASS\n");
    return 0;
}
