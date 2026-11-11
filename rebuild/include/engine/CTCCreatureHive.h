#ifndef FABLE_ENGINE_CTCCREATUREHIVE_H
#define FABLE_ENGINE_CTCCREATUREHIVE_H
// NEW (agentNF11, row 0x0077D230 CTCCreatureHive::Open): the creature-hive component. RTTI .?AVCTCCreatureHive@@ : CTCBase
// (ghidra_out/rtti_bases.tsv). Members: Ego_r.pdb CTCCreatureHive (struct_layouts_egor.tsv, 0x10): Animating +0x0c (retail
// `mov byte ptr [esi+0Ch],1` at 0x0077D30A). Methods: the Ego_r.pdb / FableWin.pdb spelling the row defines (?Open@CTCCreatureHive@@QAEXXZ,
// public non-virtual). Needs FABLE_RETAIL_CTCBASE.
#include <stddef.h>
#include "rebuild_abi.h"
#include "engine/CTCBase.h"
class CTCCreatureHive : public CTCBase {
public:
    void Open();                                                         // 0x0077D230
private:
    bool Animating;                                                      // +0x0c
};
FABLE_STATIC_ASSERT(sizeof(CTCCreatureHive) == 0x10);
#endif
