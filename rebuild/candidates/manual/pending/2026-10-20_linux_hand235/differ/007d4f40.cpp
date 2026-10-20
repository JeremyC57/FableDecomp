// CTCTavernGameCardSorting::EndRound() -- card-base EndRound; in the timed game type, advance the card layout, then either start the next round (state 7) or finish: won when the counter is within the target time, the score is the counter, a new best time is flagged
// Grade: DIFFER(ours v retail) -- 295v295: scheduling only, the def release stores the decremented count before copying it (retail `mov [eax],esi ; mov edx,esi`, ours the reverse); same gap as 0x007D6150
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
//        survives because the stats reads may alias it; the stats pointer is left uninitialised.
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
        DecRef();
        if (GetRefCount() == 0)
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
