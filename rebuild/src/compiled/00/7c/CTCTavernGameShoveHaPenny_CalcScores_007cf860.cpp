// CTCTavernGameShoveHaPenny::CalcScores() -- recomputes the score: each coin that lies wholly inside a board region band (between the region's start line and the next one) scores the lowest such region's score
// Leaf: CalcScores
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
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
// NF49 (round 7): the def classes come from the shared headers (were local CShoveHaPennyRegionDef / CRegionVector / CTavernGameShoveHaPennyDef / CShoveHaPennyDefPointer / CDefClassBase models and a same-TU
// stand-in GetSubDef body over made-up FindRef_CDefClassBase_GetSubDef_0 callees): the call at 0x007D1260 is the member template
// ??$GetSubDef@VCTavernGameShoveHaPennyDef@@@CDefClassBase@@QBE_NPAV?$CDefPointer@$$CBVCTavernGameShoveHaPennyDef@@@@@Z (Ego_r.pdb / FableWin.pdb), its body visible
// and kept out of line (FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_NOINLINE, the 0x0066BC10 idiom).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_NOINLINE
#include "fable_compiler.h"
#include "fable_string.h"
#include "engine/CDefClassBase.h"
#include "engine/CDefPointer.h"
#include "engine/CTavernGameDef.h"
#include "engine/CTavernGameShoveHaPennyDef.h"
#include "engine/CDefClassBase_GetSubDef.h"
class CThing;
class C2DVector { public: float X, Y; };
#include "engine/C3DVector.h"

class Coin { public:                                   // 0x3c bytes (packed in retail; only floats are read here)
    long      _pad_0x00;
    C2DVector Pos;                              // +0x04
    unsigned char _pad_0x0c[0x24];
    float     Radius;                           // +0x30
    long      Owner;                            // +0x34
    unsigned long Sound;                        // +0x38
};



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
    CDefPointer<const CTavernGameShoveHaPennyDef> def;
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