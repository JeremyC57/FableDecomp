// Behaviour test for NScript::CQ_AmbushTradersScript::CQ_AmbushTradersScript 0x00CF9350 (standalone copy of the model and logic).
#include <cstdio>
#include <cstdlib>
static int g_timers = 0, g_strings = 0;
class CGameScriptInterface { public: long RegisterTimer() { return 100 + g_timers++; } };
static CGameScriptInterface g_iface;
static CGameScriptInterface* g_pGameScriptInterface = &g_iface;
class CTimer { public: long TimerIndex; CTimer() : TimerIndex(g_pGameScriptInterface->RegisterTimer()) { } };
class CCharString { public: void* PStringData; CCharString() : PStringData(0) { ++g_strings; } };
class CScriptThing { public: void* vt; void* Data; void* Info; };
template <class T> struct vec { T* s; T* f; T* e; vec() : s(0), f(0), e(0) { } };
struct node { bool color; node* parent; node* left; node* right; CCharString key; bool val; };
struct flagmap {
    node* header; unsigned int count;
    flagmap() : header(0) { header = (node*)malloc(sizeof(node)); count = 0;
        header->color = false; header->parent = 0; header->left = header; header->right = header; }
};
class CScriptBase { public: int base[16]; };
struct CAmbush : CScriptBase {
    void* Game; void* MasterScriptData;
    CTimer SpeechBanditTimer, SpeechSpyTimer;
    vec<CScriptThing> v[6];
    CCharString speech[10][5];
    flagmap CSFlagMap;
    vec<int> TeleportInfo;
    CAmbush(void* data, void* game) : Game(game), MasterScriptData(data) { }
};
int main()
{
    int ok = 1; int data, game;
    CAmbush* a = new CAmbush(&data, &game);
    ok &= a->Game == &game && a->MasterScriptData == &data;
    ok &= a->SpeechBanditTimer.TimerIndex == 100 && a->SpeechSpyTimer.TimerIndex == 101 && g_timers == 2;
    for (int i = 0; i < 6; ++i) ok &= a->v[i].s == 0 && a->v[i].f == 0 && a->v[i].e == 0;
    ok &= g_strings == 50 && a->speech[9][4].PStringData == 0;
    node* h = a->CSFlagMap.header;
    ok &= h != 0 && a->CSFlagMap.count == 0 && h->color == false && h->parent == 0 && h->left == h && h->right == h;
    ok &= a->TeleportInfo.s == 0 && a->TeleportInfo.e == 0;
    free(h); delete a;
    printf(ok ? "00cf9350_TEST PASS\n" : "00cf9350_TEST FAIL\n");
    return ok ? 0 : 1;
}