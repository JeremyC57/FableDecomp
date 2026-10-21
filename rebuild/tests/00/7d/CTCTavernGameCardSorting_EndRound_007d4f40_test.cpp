// Behaviour test for 007d4f40: src/007d4f40.cpp copied verbatim (the function body and its same-TU stand-ins), the remaining
// declared callees stubbed below so calls and arguments are observable. CTCTavernGameCardSorting::EndRound()
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// CTCTavernGameCardSorting::EndRound() -- card-base EndRound; in the timed game type, advance the card layout, then either start the next round (state 7) or finish: won when the counter is within the target time, the score is the counter, a new best time is flagged
// Leaf: ?EndRound@CTCTavernGameCardSorting
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007D4F40, 295 bytes (plain `ret`); CTCTavernGameCardSorting vtable 0x0126F5B4
// slot 33 (the CardPairs override 0x007D6150 has the same skeleton).
// Callees: CTCTavernGameCardBase::EndRound() 0x008F1720 (non-virtual base call); CTCTavernGameCardBase::GetCardDef(
//          CDefPointer<const CTavernGameCardBaseDef>*) 0x008F0B20; CTCTavernGame::GetHeroStats(CTCHeroStats**) 0x005EB4B0;
//          CTCTavernGame::SetState(ETavernGameStates) 0x005E9B20; the def's virtual slot 1 when its count reaches zero.
// Layout (retail): CTCTavernGame GameWon +0x21, NumRounds +0x24, GameFinished +0x28, Score +0x2c, TargetTime +0x98,
//          NewBestScore +0xa0; CTCTavernGameCardBase Counter +0x154, GameType +0x300, CardOrder +0x304;
//          CTavernGameCardBaseDef CardOrders +0x6c (0x48-byte records); CTCHeroStats::BestTimeSorting +0x17c (PDB +0x194).
// Shape: one-case `switch` on the game type (`dec eax ; jne`); the layout index is incremented in place then reduced
//        modulo the signed layout count; `GameWon = Counter <= TargetTime` is `test ah,41h ; jp`; the constant 1 lives in
//        edx for the round compare, the clamp and the bool stores; the GameFinished store before the best-time test
//        survives because the stats reads may alias it; the stats pointer is left uninitialised. The def release is
//        `if (--m_count == 0) Destroy();` (count stored before the copy that is tested; DecRef() then GetRefCount()
//        copies first).
struct CCard;
struct C2DVector { float X, Y; };

template <class T> struct CRetailVector {       // STLport vector (12 bytes)
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    long size() const { return _M_finish - _M_start; }
};
struct CCardPositionsDef {
    unsigned char _defbase[0x3c];               // CDefClassBase (retail 0x3c)
    CRetailVector<C2DVector> Offset;            // +0x3c
};
struct CRefCounted {
    long m_count;
    void DecRef() { --m_count; }
    long GetRefCount() const { return m_count; }
};
struct CDefVirtual {
    virtual void v00();
    virtual void Destroy();                     // slot 1
};
struct CTavernGameCardBaseDef : CDefVirtual, CRefCounted {
    unsigned char _pad_0x08[0x64];
    CRetailVector<CCardPositionsDef> CardOrders;   // +0x6c
    void ReleaseTemporary()
    {
        if (--m_count == 0)
            Destroy();
    }
};
struct CDefPointer {
    CTavernGameCardBaseDef* Object;
    CDefPointer() : Object(0) {}
    ~CDefPointer() { if (Object) Object->ReleaseTemporary(); }
    CTavernGameCardBaseDef* operator->() const { return Object; }
};

struct CTCHeroStats {                           // retail layout (PDB - 0x18 here)
    unsigned char _pad[0x178];
    float BestTimePairs;                        // +0x178
    float BestTimeSorting;                      // +0x17c
};

struct CTCTavernGameCardBase {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14();
    virtual void v15(); virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19();
    virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23(); virtual void v24();
    virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29();
    virtual void v30(); virtual void v31(); virtual void v32();
    virtual void EndRound();                    // slot 33
    virtual void v34(); virtual void v35(); virtual void v36(); virtual void v37(); virtual void v38();
    virtual void v39(); virtual void v40(); virtual void v41(); virtual void v42(); virtual void v43();
    virtual void v44(); virtual void v45(); virtual void v46(); virtual void v47(); virtual void v48();
    virtual void v49(); virtual void v50(); virtual void v51(); virtual void v52(); virtual void v53();
    virtual void v54(); virtual void v55(); virtual void v56(); virtual void v57(); virtual void v58();
    virtual void v59();
    virtual void ShuffleCards();                // slot 60
    unsigned char _pad_0x04[0x1d];
    bool          GameWon;                      // +0x21
    unsigned char _pad_0x22[2];
    long          NumRounds;                    // +0x24
    bool          GameFinished;                 // +0x28
    unsigned char _pad_0x29[3];
    float         Score;                        // +0x2c
    unsigned char _pad_0x30[0x68];
    float         TargetTime;                   // +0x98
    unsigned char _pad_0x9c[4];
    bool          NewBestScore;                 // +0xa0
    unsigned char _pad_0xa1[0xb3];
    float         Counter;                      // +0x154
    unsigned char _pad_0x158[0x1a8];
    long          GameType;                     // +0x300
    long          CardOrder;                    // +0x304
    void GetCardDef(CDefPointer* out);          // 0x008F0B20
    void GetHeroStats(CTCHeroStats** stats);    // 0x005EB4B0 (CTCTavernGame)
    void SetState(long state);                  // 0x005E9B20 (CTCTavernGame, ETavernGameStates)
};
struct CTCTavernGameCardSorting : CTCTavernGameCardBase {
    virtual void EndRound();
};

void CTCTavernGameCardSorting::EndRound()
{
    CTCTavernGameCardBase::EndRound();
    CDefPointer def;
    GetCardDef(&def);
    CTCHeroStats* stats;
    GetHeroStats(&stats);
    switch (GameType) {
    case 1:
        ++CardOrder;
        CardOrder %= def->CardOrders.size();
        if (NumRounds > 1 && !GameFinished) {
            --NumRounds;
            SetState(7);
            return;
        }
        if (NumRounds < 1)
            NumRounds = 1;
        if (!GameFinished)
            GameWon = Counter <= TargetTime;
        Score = Counter;
        GameFinished = true;
        if (GameWon && (stats->BestTimeSorting == 0.0f || Counter < stats->BestTimeSorting))
            NewBestScore = true;
        break;
    }
    GameFinished = false;
    SetState(8);
}

// ---- stubs and driver ----
void CDefVirtual::v00() {}
void CTCTavernGameCardBase::v00() {}
void CTCTavernGameCardBase::v01() {}
void CTCTavernGameCardBase::v02() {}
void CTCTavernGameCardBase::v03() {}
void CTCTavernGameCardBase::v04() {}
void CTCTavernGameCardBase::v05() {}
void CTCTavernGameCardBase::v06() {}
void CTCTavernGameCardBase::v07() {}
void CTCTavernGameCardBase::v08() {}
void CTCTavernGameCardBase::v09() {}
void CTCTavernGameCardBase::v10() {}
void CTCTavernGameCardBase::v11() {}
void CTCTavernGameCardBase::v12() {}
void CTCTavernGameCardBase::v13() {}
void CTCTavernGameCardBase::v14() {}
void CTCTavernGameCardBase::v15() {}
void CTCTavernGameCardBase::v16() {}
void CTCTavernGameCardBase::v17() {}
void CTCTavernGameCardBase::v18() {}
void CTCTavernGameCardBase::v19() {}
void CTCTavernGameCardBase::v20() {}
void CTCTavernGameCardBase::v21() {}
void CTCTavernGameCardBase::v22() {}
void CTCTavernGameCardBase::v23() {}
void CTCTavernGameCardBase::v24() {}
void CTCTavernGameCardBase::v25() {}
void CTCTavernGameCardBase::v26() {}
void CTCTavernGameCardBase::v27() {}
void CTCTavernGameCardBase::v28() {}
void CTCTavernGameCardBase::v29() {}
void CTCTavernGameCardBase::v30() {}
void CTCTavernGameCardBase::v31() {}
void CTCTavernGameCardBase::v32() {}
void CTCTavernGameCardBase::v34() {}
void CTCTavernGameCardBase::v35() {}
void CTCTavernGameCardBase::v36() {}
void CTCTavernGameCardBase::v37() {}
void CTCTavernGameCardBase::v38() {}
void CTCTavernGameCardBase::v39() {}
void CTCTavernGameCardBase::v40() {}
void CTCTavernGameCardBase::v41() {}
void CTCTavernGameCardBase::v42() {}
void CTCTavernGameCardBase::v43() {}
void CTCTavernGameCardBase::v44() {}
void CTCTavernGameCardBase::v45() {}
void CTCTavernGameCardBase::v46() {}
void CTCTavernGameCardBase::v47() {}
void CTCTavernGameCardBase::v48() {}
void CTCTavernGameCardBase::v49() {}
void CTCTavernGameCardBase::v50() {}
void CTCTavernGameCardBase::v51() {}
void CTCTavernGameCardBase::v52() {}
void CTCTavernGameCardBase::v53() {}
void CTCTavernGameCardBase::v54() {}
void CTCTavernGameCardBase::v55() {}
void CTCTavernGameCardBase::v56() {}
void CTCTavernGameCardBase::v57() {}
void CTCTavernGameCardBase::v58() {}
void CTCTavernGameCardBase::v59() {}
inline void* operator new(unsigned int, void* p) { return p; }
static int g_baseEnd = 0, g_shuffle = 0, g_destroyed = 0; static long g_states[8]; static int g_stateCount = 0;
void CTCTavernGameCardBase::EndRound() { ++g_baseEnd; }
void CTCTavernGameCardBase::ShuffleCards() { ++g_shuffle; }
void CDefVirtual::Destroy() { ++g_destroyed; }
static CTavernGameCardBaseDef g_defObject;
static CTavernGameCardBaseDef& g_def = g_defObject;
void CTCTavernGameCardBase::GetCardDef(CDefPointer* out) { out->Object = &g_def; ++g_def.m_count; }
static unsigned char g_statsStore[sizeof(CTCHeroStats)];
static CTCHeroStats& g_stats = *(CTCHeroStats*)g_statsStore;
void CTCTavernGameCardBase::GetHeroStats(CTCHeroStats** stats) { *stats = &g_stats; }
void CTCTavernGameCardBase::SetState(long state) { g_states[g_stateCount++ & 7] = state; }
int main()
{
    static CCardPositionsDef orders[3];
    g_def.m_count = 1; g_def.CardOrders._M_start = orders; g_def.CardOrders._M_finish = orders + 3;
    static unsigned char gs[sizeof(CTCTavernGameCardSorting)]; memset(gs, 0, sizeof gs);
    CTCTavernGameCardSorting* g = new (gs) CTCTavernGameCardSorting;
    g->GameType = 0; g->GameFinished = true;
    g->EndRound();
    CHECK(g_baseEnd == 1); CHECK(g->GameFinished == false); CHECK(g_stateCount == 1); CHECK(g_states[0] == 8); CHECK(g_def.m_count == 1);
    g->GameType = 1; g->CardOrder = 2; g->NumRounds = 3; g->GameFinished = false;
    g->EndRound();
    CHECK(g->CardOrder == 0); CHECK(g->NumRounds == 2); CHECK(g_states[1] == 7); CHECK(g_def.m_count == 1);
    g->NumRounds = 1; g->Counter = 5.0f; g->TargetTime = 10.0f; g_stats.BestTimeSorting = 0.0f; g->NewBestScore = false;
    g->EndRound();
    CHECK(g->CardOrder == 1); CHECK(g->NumRounds == 1); CHECK(g->GameWon == true); CHECK(g->Score == 5.0f);
    CHECK(g->NewBestScore == true); CHECK(g->GameFinished == false); CHECK(g_states[2] == 8);
    g->NumRounds = 0; g->Counter = 12.0f; g->NewBestScore = false; g_stats.BestTimeSorting = 3.0f;
    g->EndRound();
    CHECK(g->NumRounds == 1); CHECK(g->GameWon == false); CHECK(g->NewBestScore == false); CHECK(g->Score == 12.0f);
    g_def.m_count = 0;                                   // the last reference goes with the def pointer
    g->GameType = 0; g->EndRound();
    CHECK(g_destroyed == 1);
    if (fails == 0) printf("007d4f40_TEST PASS\n"); else printf("007d4f40_TEST FAIL\n");
    return fails;
}