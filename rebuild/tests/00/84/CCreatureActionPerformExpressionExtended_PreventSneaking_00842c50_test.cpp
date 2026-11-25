// Behaviour test for 0x00842C50 CCreatureAction_PerformExpressionExtended::PreventSneaking: Assign("Expression_Picklock") (retail 0x0125EAC8) on the field at PExpressionDef_Object + 0x78.
// NF418 (2026-11-25): linked behaviour test: the row function is declared here, not defined, so the test links the row object and the
// callees below are defined under the row's spellings; it asserts the literal text the row passes (a mutated literal fails).
#include "engine/CCreatureAction_PerformExpressionExtended.h"
#include "rebuild_abi.h"
#include <cstdio>
#include <cstring>
struct CExprField { void Assign(const char *name); };
struct CCreatureAction_PerformExpressionExtended_Methods : CCreatureAction_PerformExpressionExtended {
    void PreventSneaking();
};
static void* g_dst = 0; static const char* g_name = 0; static int g_calls = 0;
void CExprField::Assign(const char *name) { g_dst = this; g_name = name; ++g_calls; }
static char g_obj[0x400]; static char g_holder[0x100];
int main()
{
    CCreatureAction_PerformExpressionExtended_Methods* o = (CCreatureAction_PerformExpressionExtended_Methods*)g_obj;
    o->PExpressionDef_Object = (CDefPointeeBase*)g_holder;
    o->PreventSneaking();
    if (g_calls != 1 || g_dst != (void*)(g_holder + 0x78) || g_name == 0 || std::strcmp(g_name, "Expression_Picklock") != 0) { std::printf("BAD\n"); return 1; }
    std::printf("EXPR842C50_OK\n"); return 0;
}
