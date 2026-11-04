// Behaviour test for 0x00783A30 CTCBoastingCrowdControl::GetCrowd: a g++ model of the parked source (the residue is codegen only).
#include <cstdio>
#include <map>
struct Crowd { bool dead; long hero; };
static std::map<unsigned long long, Crowd*> crowds;
static bool GetCrowd(unsigned long long hero, Crowd** out, bool create, Crowd* fresh)
{
    std::map<unsigned long long, Crowd*>::iterator it = crowds.find(hero);
    if (it != crowds.end()) { *out = it->second; if (*out && !(*out)->dead) return true; crowds.erase(it); }
    if (create) { *out = fresh; fresh->hero = (long)hero; crowds.insert(std::make_pair(hero, fresh)); return true; }
    return false;
}
int main() {
    Crowd a = { false, 0 }, b = { false, 0 }; Crowd* got = 0;
    int ok = !GetCrowd(7, &got, false, &a) && GetCrowd(7, &got, true, &a) && got == &a && a.hero == 7;
    ok &= GetCrowd(7, &got, false, &b) && got == &a;
    a.dead = true; ok &= GetCrowd(7, &got, true, &b) && got == &b && crowds.size() == 1;
    std::printf(ok ? "00783a30_TEST PASS\n" : "00783a30_TEST FAIL\n");
    return ok ? 0 : 1;
}
