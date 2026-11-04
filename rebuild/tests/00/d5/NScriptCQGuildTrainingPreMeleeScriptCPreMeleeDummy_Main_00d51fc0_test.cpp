// Behaviour test for CPreMeleeDummy::Main @ 0x00D51FC0: untargetable until mode 1; in mode 1 every hero hit wobbles and
// counts; in mode 2 a stick hit counts and spins, any other hero hit only wobbles.
#include <cstdio>
#include <string>
#include <vector>
struct Step { int Mode; int Hit; };   // Hit: 0 none, 1 hero, 2 hero with stick
struct Dummy {
    const Step* Steps; int N, At, DummyHits; bool Targetable; std::vector<std::string> Anims;
    int Mode() const { return At < N ? Steps[At].Mode : 0; }
    int Hit() const { return Steps[At - 1].Hit; }
    void Frame() { ++At; }
    void Main()
    {
        Targetable = false;
        while (Mode() != 1) Frame();
        Targetable = true;
        while (Mode() == 1) { Frame(); if (Hit() != 0) { Anims.push_back("WOBBLE"); ++DummyHits; } }
        while (Mode() == 2) {
            Frame();
            if (Hit() == 2) { ++DummyHits; Anims.push_back("GET_HIT_SPIN"); }
            else if (Hit() == 1) Anims.push_back("WOBBLE");
        }
    }
};
int main()
{
    Step s[] = { { 0, 1 }, { 1, 1 }, { 1, 0 }, { 2, 2 }, { 2, 1 }, { 0, 0 } };
    Dummy d = { s, 6, 0, 0, false, std::vector<std::string>() };
    d.Main();
    bool ok = d.Targetable && d.DummyHits == 2 && d.Anims.size() == 3 && d.Anims[0] == "WOBBLE" && d.Anims[1] == "GET_HIT_SPIN" &&
              d.Anims[2] == "WOBBLE";
    if (!ok) { std::printf("00d51fc0_TEST FAIL\n"); return 1; }
    std::printf("00d51fc0_TEST PASS\n");
    return 0;
}