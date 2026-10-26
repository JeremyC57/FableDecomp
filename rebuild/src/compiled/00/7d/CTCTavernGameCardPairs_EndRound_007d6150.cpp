// CTCTavernGameCardPairs::EndRound() -- card-base EndRound, reshuffle; in the timed game type, advance the card layout, then either start the next round (state 7) or finish: the score is the round counter and a new best time is flagged
// Leaf: ?EndRound@CTCTavernGameCardPairs
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007D6150, 262 bytes (plain `ret`); CTCTavernGameCardPairs vtable 0x0126F6FC
// slot 33 (the EndRound slot: CTCTavernGameCardBase::EndRound 0x008F1720 is called first; the manifest says "vfunc_33").
// Callees: CTCTavernGameCardBase::EndRound() 0x008F1720 (non-virtual base call); CTCTavernGameCardBase::GetCardDef(
//          CDefPointer<const CTavernGameCardBaseDef>*) 0x008F0B20; ShuffleCards (slot 60, `call [edx+0F0h]`);
//          CTCTavernGame::GetHeroStats(CTCHeroStats**) 0x005EB4B0; CTCTavernGame::SetState(ETavernGameStates) 0x005E9B20;
//          the def's virtual slot 1 when its count reaches zero.
// Layout (retail): CTCTavernGame GameWon +0x21, NumRounds +0x24, GameFinished +0x28, Score +0x2c (PDB - 8), NewBestScore
//          +0xa0 (PDB +0xbc); CTCTavernGameCardBase Counter +0x154, GameType +0x300, CardOrder +0x304;
//          CTavernGameCardBaseDef CardOrders +0x6c (0x48-byte records); CTCHeroStats::BestTimePairs +0x178 (PDB +0x190).
// Release: the def pointer's release is `if (--m_count == 0) Destroy();` (count stored before the copy that is tested;
//          DecRef() then GetRefCount() copies first).
// Shape: the game-type test is a one-case `switch` (`dec eax ; jne`); the layout index is incremented in place, then reduced modulo the signed layout count (`cdq ; idiv`); the
//        next-round arm returns through SetState(7) and the finish arm falls into the shared SetState(8), so the two
//        calls tail-merge on `push 7 / push 8`; `(GameWon && best == 0.0f) || Counter < best` is the fucompp/jnp then
//        fcomp/`test ah,5` pair; the stats pointer is zero-initialised.
class CCard;
class C2DVector { public: float X, Y; };

template <class T> struct CRetailVector {       // STLport vector (12 bytes)
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    long size() const { return _M_finish - _M_start; }
};
class CCardPositionsDef { public:
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
class CTavernGameCardBaseDef : public CDefVirtual, public CRefCounted { public:
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

class CTCHeroStats { public:                           // retail layout (PDB - 0x18 here)
    unsigned char _pad[0x178];
    float BestTimePairs;                        // +0x178
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
    unsigned char _pad_0x30[0x70];
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
struct CTCTavernGameCardPairs : CTCTavernGameCardBase {
    virtual void EndRound();
};

void CTCTavernGameCardPairs::EndRound()
{
    CTCTavernGameCardBase::EndRound();
    CDefPointer def;
    GetCardDef(&def);
    ShuffleCards();
    CTCHeroStats* stats = 0;
    GetHeroStats(&stats);
    switch (GameType) {
    case 1:
        ++CardOrder;
        CardOrder %= def->CardOrders.size();
        if (--NumRounds > 0 && !GameFinished) {
            SetState(7);
            return;
        }
        Score = Counter;
        if ((GameWon && stats->BestTimePairs == 0.0f) || Counter < stats->BestTimePairs)
            NewBestScore = true;
        break;
    }
    SetState(8);
}