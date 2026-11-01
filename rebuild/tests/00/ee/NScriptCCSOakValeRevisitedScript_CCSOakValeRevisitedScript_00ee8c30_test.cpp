// Test for NScript::CCS_OakValeRevisitedScript::~CCS_OakValeRevisitedScript() -- the implicit destructor: the seven scripted-thing resources v7..v1 go first, then the OakValeFlag map erases its nodes, then the Fires and FirePoint vectors free their storage, and the CLevelScriptBase base runs last.
#include <cstdio>
#include <cstring>
#include <cstdlib>
static char g_trace[64]; static int g_n = 0;
static void mark(char c) { g_trace[g_n++] = c; g_trace[g_n] = 0; }
struct CLevelScriptBase { virtual ~CLevelScriptBase() { mark('B'); } };
struct CVector { char id; void* start; ~CVector() { if (start) std::free(start); mark(id); } };
struct CMapNode { CMapNode* next; };
class CMap { public: CMapNode* nodes; ~CMap() { while (nodes) { CMapNode* n = nodes; nodes = n->next; std::free(n); } mark('M'); } };
class CResource { public: char id; ~CResource() { mark(id); } };
struct CCS_OakValeRevisitedScript : CLevelScriptBase {
    CVector FirePoint, Fires;
    CMap OakValeFlag;
    CResource v[7];
};
int main()
{
    CCS_OakValeRevisitedScript* s = new CCS_OakValeRevisitedScript;
    s->FirePoint.id = 'P'; s->FirePoint.start = std::malloc(8);
    s->Fires.id = 'F'; s->Fires.start = 0;
    s->OakValeFlag.nodes = (CMapNode*)std::malloc(sizeof(CMapNode)); s->OakValeFlag.nodes->next = 0;
    for (int i = 0; i < 7; ++i) s->v[i].id = (char)('1' + i);
    CLevelScriptBase* b = s;
    delete b;
    if (std::strcmp(g_trace, "7654321MFPB") != 0) { std::printf("00ee8c30_TEST FAIL\n"); return 1; }
    std::printf("00ee8c30_TEST PASS\n");
    return 0;
}