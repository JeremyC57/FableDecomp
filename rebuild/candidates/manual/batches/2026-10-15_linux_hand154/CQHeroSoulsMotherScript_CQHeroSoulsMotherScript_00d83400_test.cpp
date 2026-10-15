// Behaviour test for 0x00d83400 (CQ_HeroSoulsMotherScript ctor): the reconstructed logic run over a host model.
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
struct Mother : ScriptObject { Map csargs; Timer NewSuperScreamerTimer; Mother(void* d, GSI* g) : ScriptObject(d, g) {} };

int main()
{
    GSI gsi; g_gsi = &gsi; int data = 7;
    Mother m(&data, &gsi);
    CHECK(m.Game == &gsi); CHECK(m.Data == &data); CHECK(m.csargs.empty());
    CHECK(m.NewSuperScreamerTimer.Index == 100); CHECK(gsi.calls == 1);
    if (g_fail) { std::printf("00d83400_TEST FAIL\n"); return 1; }
    std::printf("00d83400_TEST PASS\n");
    return 0;
}
