// Behaviour test for CTCWeaponTrail::SetLastProjectileWeaponTrailPoint (0x0076E290, parked DIFFER row): a model of the row -- the newest trail entry becomes (base, tip) from the point; an empty trail is untouched.
#include <stdio.h>
#include <vector>
struct V { float X, Y, Z; };
struct Pair { V Base, Tip; };
static void SetLast(std::vector<Pair>& trail, const V& p) { if (trail.empty()) return; Pair& e = trail.back(); e.Base = p; e.Tip = p; }
static int Check() { std::vector<Pair> t; V p = { 1, 2, 3 }; SetLast(t, p); int ok = t.empty(); Pair z = { { 0, 0, 0 }, { 0, 0, 0 } }; t.push_back(z); t.push_back(z); SetLast(t, p); return ok && t[1].Base.Y == 2 && t[1].Tip.Z == 3 && t[0].Base.X == 0; }
int main()
{
    int ok = Check();
    printf(ok ? "0076e290_TEST PASS\n" : "0076e290_TEST FAIL\n");
    return ok ? 0 : 1;
}
