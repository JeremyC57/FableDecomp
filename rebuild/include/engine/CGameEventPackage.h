#ifndef FABLE_ENGINE_CGAMEEVENTPACKAGE_H
#define FABLE_ENGINE_CGAMEEVENTPACKAGE_H
#include "engine/CGameEvent.h"
// Retail opt-in (Codex pass 17): both PDBs 648h class with private stamp/count
// and forty events; native timestamp reads +0, donor accessor 018A8340.
#ifdef FABLE_RETAIL_CGAMEEVENTPACKAGE
class CGameEventPackage {
public:
    long GetTimeStamp() const { return TimeStamp; }
#ifdef FABLE_RETAIL_CGAMEEVENTPACKAGE_GETEVENTS
    // Nested opt-in (NF52, row 0x00434A60 CDisplayEngine::ConfirmFeedbackGameEventPackage): Ego_r.pdb / FableWin.pdb
    // ?GetNoEvents@CGameEventPackage@@QBEKXZ and ?GetEvent@CGameEventPackage@@QBEPBVCGameEvent@@K@Z, called out of line in retail
    // (0x009F16E0 / 0x009F16D0). Declarations only, no layout change.
    unsigned long GetNoEvents() const;                                  // 0x009F16E0
    const CGameEvent* GetEvent(unsigned long index) const;              // 0x009F16D0
#endif
private:
    long TimeStamp;
    unsigned long NoEvents;
    CGameEvent Events[40];
};
FABLE_STATIC_ASSERT_AT(15, sizeof(CGameEventPackage)==0x648);
#endif
#endif
