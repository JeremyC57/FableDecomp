// behaviour test for 0x0044A1A0 CPlayerManager::CreatePlayer (model: skip existing players, append, fill the init block)
#include <cstdio>
#include <vector>
struct Init { void* Component; void* World; void* Defs; long Number; long DefType; bool Local; void* Manager; void* Interface; bool Character; };
struct Player { Init Got; };
struct Manager {
    void* Component; void* Defs; void* World; void* Interface;
    std::vector<Player*> Players;
    bool IsPlayer(long n) const { for (unsigned i = 0; i < Players.size(); ++i) if (Players[i]->Got.Number == n) return true; return false; }
    long DefIndex(long n) const { return 100 + n; }
    void CreatePlayer(long n)
    {
        if (IsPlayer(n)) return;
        Players.push_back(new Player);
        Player* p = Players.back();
        Init init;
        init.World = 0; init.Number = -1; init.DefType = 0; init.Local = false; init.Manager = 0; init.Interface = 0; init.Character = false;
        init.Component = Component; init.Defs = Defs; init.World = World; init.Manager = this;
        init.DefType = DefIndex(n); init.Number = n;
        if (n == 4) { init.Local = false; init.Character = false; init.Interface = 0; }
        else { init.Interface = Interface; init.Local = true; init.Character = true; }
        p->Got = init;
    }
};
int main()
{
    int comp, defs, world, iface;
    Manager m; m.Component = &comp; m.Defs = &defs; m.World = &world; m.Interface = &iface;
    m.CreatePlayer(1); m.CreatePlayer(4); m.CreatePlayer(1);
    bool ok = m.Players.size() == 2;
    const Init& a = m.Players[0]->Got; const Init& b = m.Players[1]->Got;
    ok = ok && a.Number == 1 && a.DefType == 101 && a.Local && a.Character && a.Interface == &iface && a.Manager == &m && a.World == &world;
    ok = ok && b.Number == 4 && !b.Local && !b.Character && b.Interface == 0 && b.Defs == &defs && b.Component == &comp;
    for (unsigned i = 0; i < m.Players.size(); ++i) delete m.Players[i];
    std::printf(ok ? "0044a1a0_TEST PASS\n" : "0044a1a0_TEST FAIL\n");
    return ok ? 0 : 1;
}