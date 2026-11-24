#ifndef FABLE_ENGINE_CTCDRUNKENNESS_H
#define FABLE_ENGINE_CTCDRUNKENNESS_H
#ifdef FABLE_RETAIL_GREETING_PASS23
#include "engine/CTCBase.h"
#include "engine/ETCInterfaceType.h"
// Layout: both original PDBs size20; native00683cf2 tests Drunk +0c.
// Original public accessor and interface C3 are also in FableWin donor01f47f51.
class CTCDrunkenness : public CTCBase {
#ifdef FABLE_RETAIL_GREETING_PASS23
public:
    // Both original PDB constructors: public CThing&; normal fixture lifetimes.
    CTCDrunkenness(CThing&);
private:
#endif
public:
    static ETCInterfaceType FABLE_FASTCALL StatGetInterfaceType() { return TCI_DRUNKENNESS; }
    bool IsDrunk() const { return Drunk; }
#ifdef FABLE_RETAIL_GOTOPUB_DRINK_PASS27
    void DrankSecs(float seconds) { const float& time = seconds; Drunkenness += time * DrunkennessDeltaPerSecondOfDrinking; }
private:
    static const float DrunkennessDeltaPerSecondOfDrinking;
public:
#endif
#ifdef FABLE_RETAIL_CTCDRUNKENNESS_DRANKFRAMES
    // NF167 opt-in (q4151, row 0x0093A8C0 CAIStateGroup_BanditSitRoundCampfire::Drink): Ego_r.pdb / FableWin.pdb
    // ?DrankFrames@CTCDrunkenness@@QAEXJ@Z (FableWin 0x02B7FA30, beside the Drink body 0x02B7F7B0; out-of-line COMDAT copy in retail at
    // 0x0093A4E0: `fild [esp+4] ; fmul [0x013BAE70] ; fadd [ecx+10h] ; fstp [ecx+10h] ; ret 4`, expanded in Drink at 0x0093AA03),
    // ?GetDrunkenness@CTCDrunkenness@@QAEMXZ (FableWin 0x02B7FA60), ?SetDrunkenness@CTCDrunkenness@@QAEXM@Z, and the private static
    // ?DrunkennessDeltaPerFrameOfDrinking@CTCDrunkenness@@0MB 0x013BAE70 (Ego_d.pdb PUB32; dynamic initializer 0x0121F260 named in
    // Ego_r.pdb 0x01011A70 / FableWin.pdb 0x03FF5740). Inline members and a static: no layout change.
    void DrankFrames(long frames) { Drunkenness += frames * DrunkennessDeltaPerFrameOfDrinking; }
    float GetDrunkenness() { return Drunkenness; }
    void SetDrunkenness(float drunkenness) { Drunkenness = drunkenness; }
private:
    static const float DrunkennessDeltaPerFrameOfDrinking;      // 0x013BAE70
public:
#endif
private:
    bool Drunk;
    float Drunkenness;
    long FrameLastUpdatedSobering;
    const float SecondsPerFrame;
    float ThisDrunkennessThreshold;
};
FABLE_STATIC_ASSERT_AT(25, sizeof(CTCDrunkenness)==0x20);
#endif
#endif
