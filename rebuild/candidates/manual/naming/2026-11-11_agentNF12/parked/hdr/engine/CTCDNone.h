#ifndef FABLE_ENGINE_CTCDNONE_H
#define FABLE_ENGINE_CTCDNONE_H
// CTCDNone (agentNF12, 2026-11-11): the no-op thing driver. RTTI .?AVCTCDNone@@ = CTCDriverBase @0 (ghidra_out/rtti_bases.tsv).
// Declares only the static factory the inventory mannequins call: Ego_r.pdb / FableWin.pdb
// ?Create@CTCDNone@@SIPAVCThing@@JABVC3DVector@@_N@Z (static __fastcall; retail 0x00835A20, landed self row; callers
// CTCInventoryClothing / CTCInventoryStats::InitialiseMannequin 0x005B384C / 0x005CD7BC).
#include "engine/CTCDriverBase.h"
class CThing;
class C3DVector;
class CTCDNone : public CTCDriverBase {
public:
    static CThing* FABLE_FASTCALL Create(long defIndex, const C3DVector& position, bool flag);   // 0x00835A20
};
#endif // FABLE_ENGINE_CTCDNONE_H
