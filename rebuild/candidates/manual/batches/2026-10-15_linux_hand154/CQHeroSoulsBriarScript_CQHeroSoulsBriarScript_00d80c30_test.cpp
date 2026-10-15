// Behaviour test for 0x00d80c30 (CQ_HeroSoulsBriarScript ctor): the reconstructed logic run over a host model.
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
struct Handle { void* ip; void* Data; void* Info; Handle() : ip(0), Data(0), Info(0) {} };
struct Briar : ScriptObject { bool PlayedBriarCS; Handle seh_BriarRose; Map csargs; Timer Taunt; Timer Here; Briar(void* d, GSI* g) : ScriptObject(d, g) {} };

int main()
{
    GSI gsi; g_gsi = &gsi; int data = 1;
    Briar b(&data, &gsi);
    CHECK(b.Game == &gsi && b.Data == &data); CHECK(b.seh_BriarRose.Data == 0 && b.seh_BriarRose.Info == 0 && b.seh_BriarRose.ip == 0);
    CHECK(b.csargs.empty()); CHECK(b.Taunt.Index == 100 && b.Here.Index == 101 && gsi.calls == 2);
    if (g_fail) { std::printf("00d80c30_TEST FAIL\n"); return 1; }
    std::printf("00d80c30_TEST PASS\n");
    return 0;
}
