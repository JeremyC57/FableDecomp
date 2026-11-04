// Behaviour test for CMurderTwistSheriffGuard::AttackHero (0x00EB3BA0), g++ model: the five game calls on Me, then release the own and the parent's (seh_Guards[ID]) handles when valid.
#include <cstdio>
#include <string>
#include <vector>
struct Handle { bool valid; void Release() { valid = false; } };
static std::vector<std::string> AttackHero(Handle& mine, Handle* guards, long id)
{
    std::vector<std::string> calls;
    calls.push_back("target"); calls.push_back("killable"); calls.push_back("damageable"); calls.push_back("pushable"); calls.push_back("movement");
    if (mine.valid) mine.Release();
    Handle& h = guards[id];
    if (h.valid) h.Release();
    return calls;
}
int main()
{
    Handle mine = { true }; Handle g[3] = { { true }, { true }, { false } };
    std::vector<std::string> c = AttackHero(mine, g, 1);
    int ok = c.size() == 5 && c[0] == "target" && c[4] == "movement" && !mine.valid && g[0].valid && !g[1].valid && !g[2].valid;
    c = AttackHero(mine, g, 2); ok = ok && !g[2].valid && g[0].valid;
    std::printf(ok ? "00eb3ba0_TEST PASS\n" : "00eb3ba0_TEST FAIL\n");
    return ok ? 0 : 1;
}
