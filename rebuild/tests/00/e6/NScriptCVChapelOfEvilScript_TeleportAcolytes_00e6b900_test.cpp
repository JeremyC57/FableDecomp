// Behaviour test for NScript::CV_ChapelOfEvilScript::TeleportAcolytes 0x00E6B900 (model of the same control flow).
#include <cstdio>
#include <cstring>
#include <vector>
struct Thing { int id; bool alive; bool IsAlive() const { return alive; } bool operator==(const Thing& o) const { return id == o.id; } };
struct Game {
    std::vector<Thing> acolytes;
    int log[16][2]; int n;            // (thing id, marker number)
    long GetAll(std::vector<Thing>& out) { out = acolytes; return (long)acolytes.size(); }
    int GetMarker(const char* name) { return std::strcmp(name, "M_COE_AcolyteTeleport1") == 0 ? 1 : 2; }
    void Teleport(const Thing& t, int marker) { log[n][0] = t.id; log[n][1] = marker; ++n; }
};
struct Script {
    Game* game; int termAfter; int calls;
    bool IsActiveThreadTerminating() { return ++calls > termAfter; }
    void TeleportAcolytes(const Thing& acolyte)
    {
        std::vector<Thing> acolytes;
        long count = game->GetAll(acolytes);
        int marker = 0;
        if (acolyte.IsAlive()) {
            if (IsActiveThreadTerminating()) return;
            for (long i = 0; i < count; ++i) {
                if (acolytes[i] == acolyte) {
                    if (IsActiveThreadTerminating()) return;
                    marker = game->GetMarker("M_COE_AcolyteTeleport1");
                    game->Teleport(acolyte, marker);
                } else {
                    if (IsActiveThreadTerminating()) return;
                    marker = game->GetMarker("M_COE_AcolyteTeleport2");
                    game->Teleport(acolytes[i], marker);
                }
            }
        } else {
            if (IsActiveThreadTerminating()) return;
            if (acolytes.size() != 0 && acolytes[0].IsAlive()) {
                if (IsActiveThreadTerminating()) return;
                marker = game->GetMarker("M_COE_AcolyteTeleport1");
                game->Teleport(acolytes[0], marker);
            }
            if (acolytes.size() > 1 && acolytes[1].IsAlive()) {
                if (IsActiveThreadTerminating()) return;
                marker = game->GetMarker("M_COE_AcolyteTeleport2");
                game->Teleport(acolytes[1], marker);
            }
        }
    }
};
int main()
{
    Thing a = {1, true}, b = {2, true}, c = {3, true}, dead = {9, false};
    Game g; g.n = 0; g.acolytes.push_back(a); g.acolytes.push_back(b); g.acolytes.push_back(c);
    Script s = {&g, 100, 0};
    s.TeleportAcolytes(b);
    if (g.n != 3 || g.log[0][0] != 1 || g.log[0][1] != 2 || g.log[1][0] != 2 || g.log[1][1] != 1 || g.log[2][0] != 3 || g.log[2][1] != 2) { std::printf("00e6b900_TEST FAIL\n"); return 1; }
    g.n = 0; g.acolytes[0].alive = false; s.calls = 0;
    s.TeleportAcolytes(dead);       // first dead -> only acolytes[1] goes to marker 2
    if (g.n != 1 || g.log[0][0] != 2 || g.log[0][1] != 2) { std::printf("00e6b900_TEST FAIL\n"); return 1; }
    g.n = 0; s.calls = 0; s.termAfter = 2;
    s.TeleportAcolytes(a);          // terminates after the first loop step
    if (g.n != 1) { std::printf("00e6b900_TEST FAIL\n"); return 1; }
    std::printf("00e6b900_TEST PASS\n");
    return 0;
}