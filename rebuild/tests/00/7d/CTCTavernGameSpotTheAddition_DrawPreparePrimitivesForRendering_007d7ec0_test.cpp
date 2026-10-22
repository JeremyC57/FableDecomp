// Behaviour test for 007d7ec0: body copied from src/007d7ec0.cpp (CTCTavernGameSpotTheAddition::vfunc_11), callees stubbed.
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
struct CRenderContext { int id; };
static int g_destroyed = 0;
struct CDefObject {
    virtual void v00() {}
    virtual void Destroy() { ++g_destroyed; }
    long m_refCount;
};
struct CDefPointer {
    CDefObject* m_p;
    CDefPointer() : m_p(0) {}
    ~CDefPointer()
    {
        CDefObject* p = m_p;
        if (p) {
            if (--p->m_refCount == 0)
                p->Destroy();
        }
    }
};
static CDefObject* g_def = 0; static int g_getSubDef = 0;
struct CDefClassBase {
    bool GetSubDef(CDefPointer* out);
};
bool CDefClassBase::GetSubDef(CDefPointer* out)
{
    ++g_getSubDef;
    if (!g_def) return false;
    ++g_def->m_refCount; out->m_p = g_def; return true;
}
struct CThing {
    unsigned char _pad[0x70];
    CDefClassBase* m_pDefinition;
};
static int g_baseDraw = 0; static CRenderContext* g_baseContext = 0;
struct CTCTavernGame {
    void* __vftable;
    CThing* m_pParentThing;
    unsigned char _pad_0x08[0x18];
    bool IsBeingPlayed;
    void DrawPreparePrimitivesForRendering(CRenderContext* context) { ++g_baseDraw; g_baseContext = context; }
};
struct CTCTavernGameSpotTheAddition : CTCTavernGame {
    void vfunc_11(CRenderContext* context);
};
void CTCTavernGameSpotTheAddition::vfunc_11(CRenderContext* context)
{
    CTCTavernGame::DrawPreparePrimitivesForRendering(context);
    if (IsBeingPlayed) {
        CDefPointer def;
        m_pParentThing->m_pDefinition->GetSubDef(&def);
    }
}
int main()
{
    static CDefObject def; static CDefClassBase classDef; static CThing thing; static CRenderContext ctx;
    thing.m_pDefinition = &classDef; def.m_refCount = 1; g_def = &def;
    CTCTavernGameSpotTheAddition game; memset(&game, 0, sizeof(game)); game.m_pParentThing = &thing;
    game.IsBeingPlayed = true; game.vfunc_11(&ctx);
    CHECK(g_baseDraw == 1 && g_baseContext == &ctx); CHECK(g_getSubDef == 1); CHECK(def.m_refCount == 1 && g_destroyed == 0);
    def.m_refCount = 0; game.vfunc_11(&ctx); CHECK(g_destroyed == 1 && g_getSubDef == 2);
    game.IsBeingPlayed = false; game.vfunc_11(&ctx); CHECK(g_baseDraw == 3 && g_getSubDef == 2);
    g_def = 0; game.IsBeingPlayed = true; game.vfunc_11(&ctx); CHECK(g_getSubDef == 3 && g_destroyed == 1);
    if (fails == 0) printf("007d7ec0_TEST PASS\n"); else printf("007d7ec0_TEST FAIL\n");
    return fails;
}