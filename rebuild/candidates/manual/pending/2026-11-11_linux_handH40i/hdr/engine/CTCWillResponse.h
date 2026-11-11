#ifndef FABLE_ENGINE_CTCWILLRESPONSE_H
#define FABLE_ENGINE_CTCWILLRESPONSE_H
// New header (chunk H40i, row 0x0078FD60 CTCDivineWrath::FrameUpdate): the static thing queries of CTCWillResponse, Ego_r.pdb
// `static bool __fastcall (const CThing*)` overloads (the member overloads ?..@CTCWillResponse@@ABE_NXZ are a different set):
//   ?IsThingNotBloodyWillableAtAll@CTCWillResponse@@SI_NPBVCThing@@@Z  Ego_r 0x007DF4A0, retail 0x007E45D0
//   ?IsThingEpicPowerExplodable@CTCWillResponse@@SI_NPBVCThing@@@Z     Ego_r 0x007DF580, retail 0x007E4680
//   ?IsThingEpicSpellable@CTCWillResponse@@SI_NPBVCThing@@@Z           Ego_r 0x007DF5E0, retail 0x007E46D0
// (callers pass the thing in ecx: 0x00790011 / 0x00790020). Declarations only; the class layout is not modelled.
#include "fable_compiler.h"
class CThing;
class CTCWillResponse {
public:
    static bool __fastcall IsThingNotBloodyWillableAtAll(const CThing* thing);   // 0x007E45D0
    static bool __fastcall IsThingEpicPowerExplodable(const CThing* thing);      // 0x007E4680
    static bool __fastcall IsThingEpicSpellable(const CThing* thing);            // 0x007E46D0
};
#endif
