#ifndef FABLE_ENGINE_CTCHEROREMEMBERCRIMES_H
#define FABLE_ENGINE_CTCHEROREMEMBERCRIMES_H
// New header (wfE11, row 0x004A3740 CWorld::UpdateRegionLoad). Retail opt-in only: FABLE_RETAIL_CTCHEROREMEMBERCRIMES.
// Class: Ego_r.pdb CTCHeroRememberCrimes : CTCBase (RegionNamesToCrimeDurationAndFine map +0x0C, STLport 0xC; HasPaidTheFine +0x18);
// interface TCI_HERO_REMEMBER_CRIMES (224). FableWin.pdb ?GetHasPaidTheFine@CTCHeroRememberCrimes@@QAE_NXZ / ?SetHasPaidTheFine@..@@QAEX_N@Z,
// inline in retail (`mov cl,[eax+18h]` 0x004A53DF; `mov byte [eax+18h],0` 0x004A5495).
#ifdef FABLE_RETAIL_CTCHEROREMEMBERCRIMES
#ifndef FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCBASE
#endif
#include "rebuild_abi.h"
#include "engine/CTCBase.h"
class CTCHeroRememberCrimes : public CTCBase {
public:
    static ETCInterfaceType FABLE_FASTCALL StatGetInterfaceType() { return TCI_HERO_REMEMBER_CRIMES; }
    bool GetHasPaidTheFine() { return HasPaidTheFine; }
    void SetHasPaidTheFine(bool paid) { HasPaidTheFine = paid; }
protected:
    unsigned char RegionNamesToCrimeDurationAndFine[0xc];               // +0x0c map<CCharString,std::pair<long,long> >
    bool HasPaidTheFine;                                                 // +0x18
};
#endif
#endif // FABLE_ENGINE_CTCHEROREMEMBERCRIMES_H
