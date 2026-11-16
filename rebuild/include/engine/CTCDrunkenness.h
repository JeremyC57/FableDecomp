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
private:
    bool Drunk;
    float Drunkenness;
    long FrameLastUpdatedSobering;
    const float SecondsPerFrame;
    float ThisDrunkennessThreshold;
};
FABLE_STATIC_ASSERT(sizeof(CTCDrunkenness)==0x20);
#endif
#endif
