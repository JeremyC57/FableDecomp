#ifndef FABLE_ENGINE_CTCSTEALTH_H
#define FABLE_ENGINE_CTCSTEALTH_H
// New shared header (chunk 732, row 0x006AA720): the retail CTCStealth over FABLE_RETAIL_CTCBASE (RTTI .?AVCTCStealth@@ : CTCBase@0).
// Members are not modelled; only Ego_r.pdb / FableWin.pdb ?IsAvailableToPerformStealthAttack@CTCStealth@@QBE_NABVCThing@@@Z
// (retail 0x00747790). StatGetInterfaceType: TCI_STEALTH (54; retail tests bit 22 of +0x24).
#ifndef FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCBASE
#endif
#include "engine/CTCBase.h"
#include "engine/ETCInterfaceType.h"
class CThing;
class CTCStealth : public CTCBase {
public:
    static ETCInterfaceType __fastcall StatGetInterfaceType() { return TCI_STEALTH; }
    bool IsAvailableToPerformStealthAttack(const CThing&) const;   // 0x00747790
};
#endif
