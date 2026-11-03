#ifndef FABLE_ENGINE_CTCCARRYING_H
#define FABLE_ENGINE_CTCCARRYING_H
// New shared header (chunk 732, CThingPlayerCreature melee rows): the retail CTCCarrying over FABLE_RETAIL_CTCBASE (RTTI
// .?AVCTCCarrying@@ : CTCBase@0). Members are not modelled; only what the rows call: Ego_r.pdb / FableWin.pdb
// ?GetThingInPrimarySlot@CTCCarrying@@QBEPAVCThing@@XZ (retail 0x006A1440, landed) and ?RemoveThingFromPrimarySlot@CTCCarrying@@QAEPAVCThing@@_N@Z
// (0x006A1E00, landed). StatGetInterfaceType: TCI_CARRYING (70; FableWin ?StatGetInterfaceType@CTCCarrying@@SI?AW4ETCInterfaceType@@XZ,
// retail tests bit 6 of +0x28).
#ifndef FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCBASE
#endif
#include "engine/CTCBase.h"
#include "engine/ETCInterfaceType.h"
class CThing;
class CTCCarrying : public CTCBase {
public:
    static ETCInterfaceType __fastcall StatGetInterfaceType() { return TCI_CARRYING; }
    CThing* GetThingInPrimarySlot() const;                   // 0x006A1440
    CThing* RemoveThingFromPrimarySlot(bool);                // 0x006A1E00
};
#endif
