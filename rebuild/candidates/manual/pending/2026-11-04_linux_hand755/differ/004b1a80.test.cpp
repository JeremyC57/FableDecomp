// Behaviour test for CQuestManager::SetBoastAsMade (0x004B1A80), g++ model.
#include <cstdio>
#include <list>
#include <string>
struct Boast { long ID; std::string Name; };
struct Msg { long type; unsigned long delay; long data; };
struct QM { std::list<Boast> Offered, Accepted; std::list<Msg> Messages; long BoastsMade; bool HeroAlive, HeroHasStats; };
static Boast* GetBoast(QM& q, long id) { for (std::list<Boast>::iterator i = q.Offered.begin(); i != q.Offered.end(); ++i) if (i->ID == id) return &*i; return 0; }
static void SetBoastAsMade(QM& q, long id)
{
    Boast* b = GetBoast(q, id);
    if (!b) return;
    if (q.HeroAlive && q.HeroHasStats) ++q.BoastsMade;
    q.Accepted.push_back(*b);
    Msg m = { 0x29, 0x32, (long)&q.Accepted.back() };
    q.Messages.push_back(m);
    for (std::list<Boast>::iterator i = q.Offered.begin(); i != q.Offered.end(); ++i) if (i->ID == id) { q.Offered.erase(i); break; }
}
int main()
{
    QM q; q.BoastsMade = 0; q.HeroAlive = true; q.HeroHasStats = true;
    Boast a = { 1, "NoArmour" }, b = { 2, "NoWeapons" };
    q.Offered.push_back(a); q.Offered.push_back(b);
    SetBoastAsMade(q, 2);
    int ok = q.Accepted.size() == 1 && q.Accepted.back().Name == "NoWeapons" && q.Offered.size() == 1 && q.BoastsMade == 1
          && q.Messages.size() == 1 && q.Messages.back().type == 0x29 && q.Messages.back().data == (long)&q.Accepted.back();
    SetBoastAsMade(q, 7);
    ok &= q.Accepted.size() == 1 && q.Messages.size() == 1;
    q.HeroAlive = false; SetBoastAsMade(q, 1);
    ok &= q.Accepted.size() == 2 && q.BoastsMade == 1 && q.Offered.empty();
    std::printf(ok ? "004b1a80_TEST PASS\n" : "004b1a80_TEST FAIL\n");
    return ok ? 0 : 1;
}
