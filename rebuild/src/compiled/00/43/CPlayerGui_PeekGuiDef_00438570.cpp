class CPlayerGuiDef;
// Literal (NF418, 2026-11-25): was extern int data (its address pushed) `g_funcInfo` (table ?g_funcInfo@@3HA -> 0x0123173C); now spelled at the use site:
//   "PLAYER_GUI_PC", retail's pooled literal at 0x0123173C (/GF-merged ??_C@, NF418 CLASSIFICATION.tsv).
//   The (int*) cast keeps the InitGuard stand-in's signature (it is CCharString::CCharString(const char*, long) 0x0099EBF0, table-bound);
//   it goes with the port (NF418 T3).

struct InitGuard
{
    void *slot;
    InitGuard(int *p, int n);
    ~InitGuard();
};

struct Builder
{
    Builder *Build();
};

extern CPlayerGuiDef *g_playerGuiDef;
extern Builder *__stdcall AcquireBuilder(InitGuard *out, CPlayerGuiDef **slot);

// Leaf: PeekGuiDef
class CPlayerGui { public: static const CPlayerGuiDef& __fastcall PeekGuiDef(); };

const CPlayerGuiDef& __fastcall CPlayerGui::PeekGuiDef()
{
    if (g_playerGuiDef == 0)
    {
        InitGuard g((int*)"PLAYER_GUI_PC", -1);
        Builder *b = AcquireBuilder(&g, &g_playerGuiDef);
        b->Build();
    }
    return *g_playerGuiDef;
}