// Behaviour test for 0x006EAB00 CCombatRing::GetAllProngs: append the address of every prong (standalone copy of the logic)
#include <cstdio>
#include <vector>
struct Prong { int id; };
struct Ring { std::vector<Prong> Prongs; void GetAllProngs(std::vector<Prong*>& out) { for (std::vector<Prong>::iterator it = Prongs.begin(); it != Prongs.end(); ++it) out.push_back(&*it); } };

int main()
{
    Ring r; for (int i = 0; i < 3; ++i) { Prong p; p.id = i; r.Prongs.push_back(p); }
    std::vector<Prong*> out; Prong x; out.push_back(&x); r.GetAllProngs(out);
    if (out.size() != 4 || out[0] != &x || out[1] != &r.Prongs[0] || out[3]->id != 2) { std::printf("006eab00_TEST FAIL\n"); return 1; }
    std::printf("006eab00_TEST PASS\n");
    return 0;
}