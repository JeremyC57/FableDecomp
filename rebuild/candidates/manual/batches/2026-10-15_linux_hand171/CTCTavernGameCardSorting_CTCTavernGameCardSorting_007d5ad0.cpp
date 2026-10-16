// CTCTavernGameCardSorting::CTCTavernGameCardSorting(CThing* owner) -- constructor: card-base ctor, own vtable, per-card time limits 2,4,..,104 s and cleared best times, round targets, target time from the def's last prize score
// Retail: Fable.exe @ 0x007D5AD0, 150 bytes (`ret 4`; vtable 0x0126F5B4 = CTCTavernGameCardSorting per rtti-vtables.tsv).
// Callees: CTCTavernGameCardBase::CTCTavernGameCardBase(CThing*) 0x008F1CD0; CTCTavernGame::GetTavernGameDef(CDefPointer*)
//          0x005EA000; the def's virtual slot 1 when its count reaches 0.
// Layout (retail, below Ego_r.pdb): CTCTavernGame::TargetTime +0x98 (PDB +0xb4); CTCTavernGameCardBase::Times[52] +0x15c,
//          BestTimes[52] +0x22c (PDB +0x180/+0x250); CTCTavernGameCardSorting::TimeLimitNumRoundsToWin +0x318,
//          TimeLimitBestNumRounds +0x31c (PDB +0x338/+0x33c). Def: PrizeScores vector (8-byte {Score, Mult}) end at +0x178.
// Shape: the loop counter is strength-reduced to 2*(i+1) and kept in the dead `owner` slot for the `fild`; the def pointer
//        released with no null test through the def's inline ReleaseTemporary().
class CThing;

struct CPrizeScoreDef { float Score; long Mult; };

struct CRefCounted {
    long m_count;
    void DecRef() { --m_count; }
    long GetRefCount() const { return m_count; }
};
struct CDefVirtual {
    virtual void v00();
    virtual void Destroy();                     // slot 1
};
struct CTavernGameDef : CDefVirtual, CRefCounted {
    unsigned char _pad_0x08[0x16c];
    CPrizeScoreDef* PrizeScoresBegin;           // +0x174
    CPrizeScoreDef* PrizeScoresEnd;             // +0x178
    void ReleaseTemporary()
    {
        DecRef();
        if (GetRefCount() == 0)
            Destroy();
    }
};
struct CDefPointer {
    CTavernGameDef* Object;
    CDefPointer() : Object(0) {}
    ~CDefPointer() { Object->ReleaseTemporary(); }
    CTavernGameDef* operator->() const { return Object; }
};

struct CTCTavernGameCardBase {
    CTCTavernGameCardBase(CThing* owner);      // 0x008F1CD0
    virtual void v00();
    void GetTavernGameDef(CDefPointer* out);   // 0x005EA000
    unsigned char _pad_0x04[0x94];
    float         TargetTime;                  // +0x98
    unsigned char _pad_0x9c[0xc0];
    float         Times[52];                   // +0x15c
    float         BestTimes[52];               // +0x22c
    unsigned char _pad_0x2fc[0x1c];
};

struct CTCTavernGameCardSorting : CTCTavernGameCardBase {
    long TimeLimitNumRoundsToWin;               // +0x318
    long TimeLimitBestNumRounds;                // +0x31c
    CTCTavernGameCardSorting(CThing* owner);
    virtual void v00();
};

CTCTavernGameCardSorting::CTCTavernGameCardSorting(CThing* owner)
    : CTCTavernGameCardBase(owner)
{
    for (long i = 0; i < 52; ++i) {
        Times[i] = (float)((i + 1) * 2);
        BestTimes[i] = 0.0f;
    }
    TimeLimitNumRoundsToWin = 1;
    TimeLimitBestNumRounds = 0;
    CDefPointer def;
    GetTavernGameDef(&def);
    TargetTime = (def->PrizeScoresEnd - 1)->Score;
}
