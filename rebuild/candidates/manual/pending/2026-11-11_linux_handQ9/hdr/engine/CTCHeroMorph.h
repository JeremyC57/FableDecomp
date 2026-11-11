#ifndef FABLE_ENGINE_CTCHEROMORPH_H
#define FABLE_ENGINE_CTCHEROMORPH_H
// Overlay header (chunk 1009b): there is no promoted engine/CTCHeroMorph.h (the generated flattened layout sits in
// engine/_quarantine/ with a reconciliation conflict). This file only carries a retail opt-in; without it the class is
// forward-declared.
#ifdef FABLE_RETAIL_CTCHEROMORPH
// Retail opt-in (chunk 1009b, row 0x0059FC17 CTCInventoryTrade::AlreadyHaveSpecial): the hero's morph component as the trade
// screen reaches it. RTTI .?AVCTCHeroMorph@@ : CTCAppearanceMorphBase : CTCBase (ghidra_out/rtti_bases.tsv); no
// CTCAppearanceMorphBase header exists, so the class derives CTCBase directly (both single-inheritance at offset 0). The GetTC key
// is CTCAppearanceMorphBase's interface (Ego_r.pdb ?StatGetInterfaceType@CTCAppearanceMorphBase@@SI?AW4ETCInterfaceType@@XZ:
// TCI_APPEARANCE_MORPH = 3, `push 3 ; pop edi` 0x0059FC99). Members (Ego_r.pdb / FableWin.pdb, out of line, both landed):
// ?GetTattoos@CTCHeroMorph@@QBEXAAV?$vector@V?$CDefPointer@$$CBVCTattooDef@@@@V?$allocator@V?$CDefPointer@$$CBVCTattooDef@@@@@std@@@std@@@Z
// 0x0071DD50 and ?HasAPermanentTattooCoveringArea@CTCHeroMorph@@QBEJXZ 0x0071C2D0. Members not modelled. Needs
// FABLE_RETAIL_DEF_CLASS_BASE + FABLE_RETAIL_CTATTOODEF (the vector element).
#include "engine/ETCInterfaceType.h"
#include "engine/CTCBase.h"
#include "engine/CDefPointer.h"
#include "engine/CTattooDef.h"
#include "stlport/fable_containers.h"
class CTCHeroMorph : public CTCBase {
public:
    static ETCInterfaceType __fastcall StatGetInterfaceType() { return TCI_APPEARANCE_MORPH; }
    void GetTattoos(std::vector<CDefPointer<const CTattooDef> >& tattoos) const;   // 0x0071DD50
    long HasAPermanentTattooCoveringArea() const;                                   // 0x0071C2D0
};
#else
class CTCHeroMorph;
#endif
#endif // FABLE_ENGINE_CTCHEROMORPH_H
