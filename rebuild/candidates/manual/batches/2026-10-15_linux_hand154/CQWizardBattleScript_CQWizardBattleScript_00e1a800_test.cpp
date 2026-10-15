// Behaviour test for 0x00e1a800 (CQ_WizardBattleScript ctor): the reconstructed logic run over a host model.
#include <cstdio>
#include <cstring>
#include <cstdlib>
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) g_fail = 1; } while (0)
struct GSI { int nextTimer; int calls; GSI() : nextTimer(100), calls(0) {} int RegisterTimer() { ++calls; return nextTimer++; } };
static GSI* g_gsi = 0;
struct Timer { int Index; Timer() : Index(g_gsi->RegisterTimer()) {} };
struct Node { char color; Node* parent; Node* left; Node* right; };
struct Map { Node* header; unsigned int count;
    Map() : header(0), count(0) { header = (Node*)std::malloc(sizeof(Node)); header->color = 0; header->parent = 0; header->left = header; header->right = header; }
    ~Map() { std::free(header); }
    bool empty() const { return header->left == header && header->right == header && header->parent == 0 && count == 0; } };
struct ScriptObject { GSI* Game; void* Data; ScriptObject(void* d, GSI* g) : Game(g), Data(d) {} };
struct ScriptThing { void* Data; void* Info; ScriptThing() : Data(0), Info(0) {} };
struct Wizard : ScriptObject { ScriptThing Maze, Top, Middle; Wizard(void* d, GSI* g) : ScriptObject(d, g) {} };

int main()
{
    GSI gsi; g_gsi = &gsi; int data = 1;
    Wizard w(&data, &gsi);
    CHECK(w.Game == &gsi && w.Data == &data);
    CHECK(!w.Maze.Data && !w.Maze.Info && !w.Top.Data && !w.Top.Info && !w.Middle.Data && !w.Middle.Info); CHECK(gsi.calls == 0);
    if (g_fail) { std::printf("00e1a800_TEST FAIL\n"); return 1; }
    std::printf("00e1a800_TEST PASS\n");
    return 0;
}
