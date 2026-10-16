// Behaviour test for 0x00e54310: CV_BookCollectingScript ctor: arguments stored, vectors empty, handles cleared
// The reconstructed logic runs over a host model of the retail classes and callees.
#include <cstdio>
#include <cstring>
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { g_fail = 1; std::printf("check failed line %d\n", __LINE__); } } while (0)
static long g_timers = 0;
struct Timer { long Index; Timer() : Index(++g_timers) {} };
struct Handle { void* Data; void* Info; Handle() : Data(0), Info(0) {} };
struct Vec3 { void* s; void* f; void* e; Vec3() : s(0), f(0), e(0) {} };
struct Node { char color; Node* parent; Node* left; Node* right; };
struct Map { Node* header; unsigned long count; Map() : header(new Node), count(0) { header->color = 0; header->parent = 0; header->left = header; header->right = header; } ~Map() { delete header; } };
struct Str { const char* p; Str() : p("") {} };
struct Base { void* game; void* data; Base(void* d, void* g) : game(g), data(d) {} };

struct Book : Base { Vec3 reactions; Handle boy, girl; Vec3 def, donated, owned; Book(void* d, void* g) : Base(d, g) {} };
static void run() { int a, b; Book x(&a, &b); CHECK(x.game == &b && x.data == &a); CHECK(!x.reactions.s && !x.boy.Data && !x.girl.Info && !x.def.f && !x.donated.e && !x.owned.s); }

int main()
{
    run();
    if (g_fail) { std::printf("00e54310_TEST FAIL\n"); return 1; }
    std::printf("00e54310_TEST PASS\n");
    return 0;
}
