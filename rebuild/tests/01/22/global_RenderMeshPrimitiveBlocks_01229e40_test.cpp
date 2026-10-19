#include <cstdio>
// Behaviour of the at-exit list destructor at 0x01229E40: clear() runs on the static list, then the
// sentinel node is handed to free (spied here: recorded only while armed, never released).
struct CRetailList_004b8050 {
    void* _M_node;
    void clear();
};
static int g_clearCalls = 0;
static CRetailList_004b8050* g_clearThis = 0;
static int g_armed = 0;
static int g_freeCalls = 0;
static void* g_freed = 0;
static char g_sentinel[12];
CRetailList_004b8050 g_List_013baef8 = { g_sentinel };
void CRetailList_004b8050::clear() { ++g_clearCalls; g_clearThis = this; }
extern "C" void __cdecl free(void* p) { if (g_armed) { ++g_freeCalls; g_freed = p; } }
void __fastcall RenderMeshPrimitiveBlocks()
{
    g_List_013baef8.clear();
    if (g_List_013baef8._M_node != 0)
        free(g_List_013baef8._M_node);
}
int main()
{
    g_armed = 1; RenderMeshPrimitiveBlocks(); g_armed = 0;
    bool ok = g_clearCalls == 1 && g_clearThis == &g_List_013baef8 && g_freeCalls == 1 && g_freed == g_sentinel;
    g_List_013baef8._M_node = 0; g_freeCalls = 0;
    g_armed = 1; RenderMeshPrimitiveBlocks(); g_armed = 0;
    ok = ok && g_clearCalls == 2 && g_freeCalls == 0;
    if (ok) { std::printf("AUTO_TINY_01229e40_TEST PASS\n"); return 0; }
    std::printf("FAIL\n");
    return 1;
}
