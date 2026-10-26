// CTCTavernGameCardPairs::CTCTavernGameCardPairs(CThing* owner) -- constructor: card-base ctor, own vtable, target score from the def's last prize score, pairs state reset
// Retail: Fable.exe @ 0x007D7520, 113 bytes (`ret 4`; vtable 0x0126F6FC = CTCTavernGameCardPairs per rtti-vtables.tsv).
// Callees: CTCTavernGameCardBase::CTCTavernGameCardBase(CThing*) 0x008F1CD0; CTCTavernGame::GetTavernGameDef(CDefPointer*)
//          0x005EA000 (a forwarder: owner->+0x70 def lookup 0x005ED4A0); the def's virtual slot 1 when its count reaches 0.
// Layout (retail, below Ego_r.pdb): +0x98 a CTCTavernGame float filled from the def's last PrizeScores entry (retail
//          CTavernGameDef PrizeScores vector at +0x174, 8-byte {Score, Mult} entries, so `end()[-8]` = back().Score);
//          +0x300 NumCards (PDB 0x320), +0x31c LastFramePointerMoved (PDB 0x33c), +0x328 PreviousDirection (PDB 0x348).
// Shape: the three stores follow the def read, so they are body assignments; the def pointer is released through an inline
//        ReleaseTemporary() with no null test (DecRef/GetRefCount on the ref-count base at +4).
class CThing;

class CPrizeScoreDef { public: float Score; long Mult; };

struct CRefCounted {
    long m_count;
    void DecRef() { --m_count; }
    long GetRefCount() const { return m_count; }
};
struct CDefVirtual {
    virtual void v00();
    virtual void Destroy();                     // slot 1
};
class CTavernGameDef : public CDefVirtual, public CRefCounted { public:
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
    float         m_targetScore;               // +0x98
    unsigned char _pad_0x9c[0x264];
    long          NumCards;                    // +0x300
    unsigned char _pad_0x304[0x14];
};

struct CTCTavernGameCardPairs : CTCTavernGameCardBase {
    void* AlternateSelectedCard;                // +0x318
    long  LastFramePointerMoved;                // +0x31c
    long  NumFramesRepeated;                    // +0x320
    bool  RepeatMovement;                       // +0x324
    long  PreviousDirection;                    // +0x328
    CTCTavernGameCardPairs(CThing* owner);
    virtual void v00();
};

CTCTavernGameCardPairs::CTCTavernGameCardPairs(CThing* owner)
    : CTCTavernGameCardBase(owner)
{
    CDefPointer def;
    GetTavernGameDef(&def);
    m_targetScore = (def->PrizeScoresEnd - 1)->Score;
    NumCards = 1;
    LastFramePointerMoved = 0;
    PreviousDirection = 0;
}