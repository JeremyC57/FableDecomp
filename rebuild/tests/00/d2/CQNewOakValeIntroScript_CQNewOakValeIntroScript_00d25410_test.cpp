// Behaviour test for 0x00d25410 (CQ_NewOakValeIntroScript ctor): the reconstructed logic run over a host model.
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
struct Zero6 { long Word[6]; Zero6() { for (int i = 0; i < 6; ++i) Word[i] = 0; } };
struct NOV : ScriptObject { Zero6 counters; Map m; Timer t90, t94; NOV(void* d, GSI* g) : ScriptObject(d, g) {} };

int main()
{
    GSI gsi; g_gsi = &gsi; int data = 1;
    NOV n(&data, &gsi);
    for (int i = 0; i < 6; ++i) CHECK(n.counters.Word[i] == 0);
    CHECK(n.m.empty()); CHECK(n.t90.Index == 100 && n.t94.Index == 101); CHECK(n.Game == &gsi && n.Data == &data);
    if (g_fail) { std::printf("00d25410_TEST FAIL\n"); return 1; }
    std::printf("00d25410_TEST PASS\n");
    return 0;
}