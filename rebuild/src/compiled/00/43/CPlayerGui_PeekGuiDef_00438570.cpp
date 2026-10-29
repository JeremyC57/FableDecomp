class CPlayerGuiDef;

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
extern int g_funcInfo;
extern Builder *__stdcall AcquireBuilder(InitGuard *out, CPlayerGuiDef **slot);

// Leaf: PeekGuiDef
class CPlayerGui { public: static const CPlayerGuiDef& __fastcall PeekGuiDef(); };

const CPlayerGuiDef& __fastcall CPlayerGui::PeekGuiDef()
{
    if (g_playerGuiDef == 0)
    {
        InitGuard g(&g_funcInfo, -1);
        Builder *b = AcquireBuilder(&g, &g_playerGuiDef);
        b->Build();
    }
    return *g_playerGuiDef;
}