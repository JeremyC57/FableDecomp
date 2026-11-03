// Behaviour test for NUISystem::CKeyRedefiner::~CKeyRedefiner (0x00558660): the selected-redefiner global is cleared
// only when it points at the redefiner being destroyed; the alias list member is destroyed with it.
#include <cstdio>
static int g_listDtors = 0;
struct AliasList { ~AliasList() { ++g_listDtors; } };
struct Clickable { virtual ~Clickable() {} };
struct KeyRedefiner;
static KeyRedefiner* g_pSelectedKeyRedefiner = 0;
struct KeyRedefiner : Clickable {
    AliasList Actions;
    ~KeyRedefiner() { if (g_pSelectedKeyRedefiner == this) g_pSelectedKeyRedefiner = 0; }
};
int main()
{
    KeyRedefiner* a = new KeyRedefiner; KeyRedefiner* b = new KeyRedefiner;
    g_pSelectedKeyRedefiner = a;
    delete b;
    if (g_pSelectedKeyRedefiner != a || g_listDtors != 1) { std::printf("00558660_TEST FAIL\n"); return 1; }
    delete a;
    if (g_pSelectedKeyRedefiner != 0 || g_listDtors != 2) { std::printf("00558660_TEST FAIL\n"); return 1; }
    std::printf("00558660_TEST PASS\n");
    return 0;
}
