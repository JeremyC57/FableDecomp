// CTCTavernGameShoveHaPenny::CalcScores() -- recomputes the score: each coin that lies wholly inside a board region band (between the region's start line and the next one) scores the lowest such region's score
// Leaf: CalcScores
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007CF860, 313 bytes (plain `ret`).
// Callees: CDefClassBase::GetSubDef<CTavernGameShoveHaPennyDef>(CDefPointer<const CTavernGameShoveHaPennyDef>*) 0x007D1260
//          (on the owner's def class, owner +0x70; this TU, see the stand-in below); __ftol2 0x00BFEA70 (the `(long)` cast);
//          the def's virtual slot 1 on release.
// Layout (retail): CTCTavernGame::Score +0x2c; CTCTavernGameCoinBase Coins +0xfc (0x3c-byte Coin: Pos +0x04, Radius
//          +0x30), BoardSize +0x130; CTavernGameShoveHaPennyDef::Regions +0x1a4 (PDB +0x1a8; 0x30-byte
//          CShoveHaPennyRegionDef: Start +0x28, Score +0x2c).
// Shape: the region loop re-evaluates `Regions.size() - 1` (unsigned `jb`/`jbe`) every pass; the coin's far edge
//        `Radius + Pos.Y` (written in retail's load order) is loop-invariant and stays on the x87 stack across the region loop; `this` is spilled and its
//        register walks the regions; the best score starts at 1000 and is added to the score only when one was found.
//        Regions[i] goes through the STLport `*(begin() + i)` operator[] with an inline begin(); a raw `*(_M_start + i)`
//        loads Pos.Y before Radius in the hoisted sum (operand order flips).
class CThing;
class C2DVector { public: float X, Y; };
class C3DVector { public: float X, Y, Z; };

class Coin { public:                                   // 0x3c bytes (packed in retail; only floats are read here)
    long      _pad_0x00;
    C2DVector Pos;                              // +0x04
    unsigned char _pad_0x0c[0x24];
    float     Radius;                           // +0x30
    long      Owner;                            // +0x34
    unsigned long Sound;                        // +0x38
};
class CShoveHaPennyRegionDef { public:                 // 0x30 bytes
    unsigned char _defbase[0x28];
    float         Start;                        // +0x28
    float         Score;                        // +0x2c
};
struct CRegionVector {                          // STLport vector<CShoveHaPennyRegionDef> (12 bytes)
    CShoveHaPennyRegionDef* _M_start;
    CShoveHaPennyRegionDef* _M_finish;
    CShoveHaPennyRegionDef* _M_end_of_storage;
    unsigned long size() const { return _M_finish - _M_start; }
    CShoveHaPennyRegionDef* begin() const { return _M_start; }
    CShoveHaPennyRegionDef& operator[](unsigned long i) const { return *(begin() + i); }
};

class CTavernGameShoveHaPennyDef { public:
    virtual void v00();
    virtual void Destroy();                     // slot 1
    long          m_count;                      // +0x04
    unsigned char _pad_0x08[0x19c];
    CRegionVector Regions;                      // +0x1a4
    void Release()
    {
        if (--m_count == 0)
            Destroy();
    }
};
struct CShoveHaPennyDefPointer {                // CDefPointer<const CTavernGameShoveHaPennyDef>
    CTavernGameShoveHaPennyDef* Object;
    CShoveHaPennyDefPointer() : Object(0) {}
    ~CShoveHaPennyDefPointer() { if (Object) Object->Release(); }
    CTavernGameShoveHaPennyDef* operator->() const { return Object; }
};

class CDefClassBase { public:
    bool GetSubDef(CShoveHaPennyDefPointer* out);   // 0x007D1260
};
CTavernGameShoveHaPennyDef* FindRef_CDefClassBase_GetSubDef_0(CDefClassBase* defClass);   // stand-in callee of the stand-in body below
// Stand-in body (the real 0x007D1260 is in this TU, out of line, see the landed CDefClassBase_GetSubDef007d1260): it only
// writes the def through the out parameter, so VC7.1 knows the caller's address-taken pointer does not escape the call.
#pragma auto_inline(off)
bool CDefClassBase::GetSubDef(CShoveHaPennyDefPointer* out)
{
    CTavernGameShoveHaPennyDef* def = FindRef_CDefClassBase_GetSubDef_0(this);
    if (!def)
        return false;
    out->Object = def;
    return true;
}
#pragma auto_inline(on)

struct CThingDefOwner {
    unsigned char  _pad_0x00[0x70];
    CDefClassBase* DefClass;                    // +0x70
};

struct CTCTavernGameShoveHaPenny {
    virtual void v00();
    CThingDefOwner* Owner;                      // +0x04
    unsigned char   _pad_0x08[0x24];
    float           Score;                      // +0x2c
    unsigned char   _pad_0x30[0xcc];
    Coin*           CoinsBegin;                 // +0xfc
    Coin*           CoinsEnd;                   // +0x100
    Coin*           CoinsEndOfStorage;          // +0x104
    unsigned char   _pad_0x108[0x28];
    C3DVector       BoardSize;                  // +0x130
    void CalcScores();
};

void CTCTavernGameShoveHaPenny::CalcScores()
{
    CShoveHaPennyDefPointer def;
    Owner->DefClass->GetSubDef(&def);
    Score = 0;
    for (Coin* coin = CoinsBegin; coin != CoinsEnd; ++coin) {
        long best = 1000;
        for (unsigned long i = 0; i < def->Regions.size() - 1; ++i) {
            if (coin->Radius + coin->Pos.Y > BoardSize.Y * def->Regions[i].Start
                && coin->Pos.Y - coin->Radius < def->Regions[i + 1].Start * BoardSize.Y
                && best > def->Regions[i].Score)
                best = (long)def->Regions[i].Score;
        }
        if (best != 1000)
            Score += best;
    }
}