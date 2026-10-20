// Behaviour test for 007d80c0: body copied from src/007d80c0.cpp (CTCTavernGameSpotTheAddition::NewGame), callees stubbed.
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
static int g_destroyed = 0;
struct CDefPointee {
    virtual void v00() {}
    virtual void Destroy() { ++g_destroyed; }
    long m_count;
    void Release()
    {
        if (--m_count == 0)
            Destroy();
    }
};
struct CTavernGameSpotTheAdditionDef : CDefPointee {};
struct CDefPointer {
    CTavernGameSpotTheAdditionDef* Object;
    CDefPointer() : Object(0) {}
    ~CDefPointer()
    {
        if (Object && --Object->m_count == 0)
            Object->Destroy();
    }
};
static CTavernGameSpotTheAdditionDef* g_def = 0; static int g_getSubDef = 0;
struct CDefClassBase {
    bool GetSubDef(CDefPointer* out);
};
bool CDefClassBase::GetSubDef(CDefPointer* out)
{
    ++g_getSubDef;
    if (!g_def) return false;
    ++g_def->m_count; out->Object = g_def; return true;
}
struct CThing {
    unsigned char _pad_0x00[0x70];
    CDefClassBase* DefHolder;
};
static const char* g_log[8]; static int g_logN = 0; static long g_state = -1; static float g_coverFraction = -1.0f;
struct CTCTavernGame {
    virtual void v00() {}
    CThing* ParentThing;
    unsigned char _pad_0x08[0x19];
    bool    GameWon;
    unsigned char _pad_0x22[0xce];
    void NewGameBase() { g_log[g_logN++] = "base"; }
    void SetState(long state) { g_log[g_logN++] = "state"; g_state = state; }
};
struct CTCTavernGameSpotTheAddition : CTCTavernGame {
    long    CurrentRound;
    float   CurrentTime;
    CThing* Cover;
    float   CoverTimer;
    bool    UpdatedKnickKnacks;
    unsigned char _pad_0x101[0x47];
    long    Selected;
    bool    FirstRound;
    void GetSpotDef(CDefPointer* out) { ParentThing->DefHolder->GetSubDef(out); }
    void PositionCover(float fraction) { g_log[g_logN++] = "cover"; g_coverFraction = fraction; }
    void NewGame();
};
void CTCTavernGameSpotTheAddition::NewGame()
{
    NewGameBase();
    CDefClassBase* defClass = ParentThing->DefHolder;
    CDefPointer def;
    defClass->GetSubDef(&def);
    CurrentTime = 0.0f;
    CurrentRound = 0;
    GameWon = false;
    UpdatedKnickKnacks = false;
    Selected = 0;
    CoverTimer = 0.0f;
    PositionCover(1.0f);
    FirstRound = true;
    SetState(7);
}
int main()
{
    static CThing thing; static CDefClassBase classDef; static CTavernGameSpotTheAdditionDef def;
    thing.DefHolder = &classDef; def.m_count = 1; g_def = &def;
    CTCTavernGameSpotTheAddition game; memset(&game, 0xAA, sizeof(game)); game.ParentThing = &thing;
    game.NewGame();
    CHECK(g_getSubDef == 1); CHECK(game.CurrentTime == 0.0f && game.CurrentRound == 0 && !game.GameWon && !game.UpdatedKnickKnacks && game.Selected == 0 && game.CoverTimer == 0.0f);
    CHECK(game.FirstRound); CHECK(g_coverFraction == 1.0f && g_state == 7);
    CHECK(g_logN == 3 && strcmp(g_log[0], "base") == 0 && strcmp(g_log[1], "cover") == 0 && strcmp(g_log[2], "state") == 0);
    CHECK(def.m_count == 1 && g_destroyed == 0);
    def.m_count = 0; game.NewGame(); CHECK(g_destroyed == 1);
    if (fails == 0) printf("007d80c0_TEST PASS\n"); else printf("007d80c0_TEST FAIL\n");
    return fails;
}