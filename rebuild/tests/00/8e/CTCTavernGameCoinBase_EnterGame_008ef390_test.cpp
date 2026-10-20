// Behaviour test for 008ef390: body copied from src/008ef390.cpp (CTCTavernGameCoinBase::EnterGame), callees stubbed.
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
struct CThingObject { int id; };
static int g_destroyed = 0;
struct CDefPointee {
    virtual void v00() {}
    virtual void Destroy() { ++g_destroyed; }
    long m_count;
};
struct CTavernGameCoinBaseDef : CDefPointee {
    unsigned char _pad_0x08[0x38];
    long ArrowDefIndex;
};
struct CDefPointer {
    CTavernGameCoinBaseDef* Object;
    CDefPointer() : Object(0) {}
};
struct CDefReference {
    CTavernGameCoinBaseDef* Object;
    CDefReference(CTavernGameCoinBaseDef* object) : Object(object) {}
    ~CDefReference()
    {
        if (--Object->m_count == 0)
            Object->Destroy();
    }
    CTavernGameCoinBaseDef* operator->() const { return Object; }
};
static CTavernGameCoinBaseDef* g_def = 0; static int g_getDef = 0;
struct CThingDefHolder {
    void GetDef(CDefPointer* out);
};
void CThingDefHolder::GetDef(CDefPointer* out)
{
    ++g_getDef;
    if (!g_def) return;
    ++g_def->m_count; out->Object = g_def;
}
struct CThing {
    unsigned char _pad_0x00[0x70];
    CThingDefHolder* DefHolder;
};
static int g_enterBase = 0; static CThingObject g_pieces[3]; static int g_created = 0; static long g_models[3];
struct CTCTavernGame {
    virtual void v00() {}
    CThing* ParentThing;
    void EnterGameBase() { ++g_enterBase; }
    CThingObject* CreateGamePiece(long model) { g_models[g_created] = model; return &g_pieces[g_created++]; }
};
struct CTCTavernGameCoinBase : CTCTavernGame {
    unsigned char _pad_0x08[0xe8];
    CThingObject* Arrow;
    CThingObject* Arrow1;
    CThingObject* Arrow2;
    void GetCoinDef(CDefPointer* out) { ParentThing->DefHolder->GetDef(out); }
    void EnterGame();
};
void CTCTavernGameCoinBase::EnterGame()
{
    EnterGameBase();
    CDefPointer out;
    GetCoinDef(&out);
    CDefReference def(out.Object);
    Arrow = CreateGamePiece(def.Object->ArrowDefIndex);
    Arrow1 = CreateGamePiece(def.Object->ArrowDefIndex);
    Arrow2 = CreateGamePiece(def.Object->ArrowDefIndex);
}
int main()
{
    static CThing thing; static CThingDefHolder holder; static CTavernGameCoinBaseDef def;
    thing.DefHolder = &holder; def.m_count = 1; def.ArrowDefIndex = 66; g_def = &def;
    CTCTavernGameCoinBase game; memset(&game, 0, sizeof(game)); game.ParentThing = &thing;
    game.EnterGame();
    CHECK(g_enterBase == 1 && g_getDef == 1); CHECK(g_created == 3 && g_models[0] == 66 && g_models[1] == 66 && g_models[2] == 66);
    CHECK(game.Arrow == &g_pieces[0] && game.Arrow1 == &g_pieces[1] && game.Arrow2 == &g_pieces[2]); CHECK(def.m_count == 1 && g_destroyed == 0);
    def.m_count = 0; g_created = 0; game.EnterGame(); CHECK(g_destroyed == 1);
    if (fails == 0) printf("008ef390_TEST PASS\n"); else printf("008ef390_TEST FAIL\n");
    return fails;
}