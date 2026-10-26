#include <cstdio>

class CTCEnemy { public: int tag; };

struct GlobalThing {
    int calls;
    int lastArg;
    void Register(int arg);   // out-of-line => real emitted symbol
};

void GlobalThing::Register(int arg) { calls++; lastArg = arg; }

static GlobalThing g_obj = {0,-1};
GlobalThing* g_electroGlobal = &g_obj;

static int g_baseCalled = 0;
static CTCEnemy* g_baseSelf = 0;

void __fastcall CTCEnemy_base_OnInitialActivate(CTCEnemy* enemy)
{
    g_baseCalled++;
    g_baseSelf = enemy;
}

void __fastcall OnInitialActivate(CTCEnemy* enemy);

int main()
{
    CTCEnemy e; e.tag = 42;
    OnInitialActivate(&e);
    if (g_obj.calls == 1 && g_obj.lastArg == 0 &&
        g_baseCalled == 1 && g_baseSelf == &e)
        printf("0076c830_TEST PASS\n");
    else
        printf("FAIL c=%d a=%d b=%d\n", g_obj.calls, g_obj.lastArg, g_baseCalled);
    return 0;
}