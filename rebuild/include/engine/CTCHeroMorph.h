#ifndef FABLE_ENGINE_CTCHEROMORPH_H
#define FABLE_ENGINE_CTCHEROMORPH_H
// Overlay header (chunk 1009b): there is no promoted engine/CTCHeroMorph.h (the generated flattened layout sits in
// engine/_quarantine/ with a reconciliation conflict). This file only carries a retail opt-in; without it the class is
// forward-declared.
#if defined(FABLE_RETAIL_CTCHEROMORPH) || defined(FABLE_RETAIL_CTCHEROMORPH_TATTOOTHINGS)
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
class CThingBaseDef;
class CTCHeroMorph : public CTCBase {
public:
    static ETCInterfaceType __fastcall StatGetInterfaceType() { return TCI_APPEARANCE_MORPH; }
#ifdef FABLE_RETAIL_CTCHEROMORPH
    void GetTattoos(std::vector<CDefPointer<const CTattooDef> >& tattoos) const;   // 0x0071DD50
    long HasAPermanentTattooCoveringArea() const;                                   // 0x0071C2D0
#endif
#ifdef FABLE_RETAIL_CTCHEROMORPH_TATTOOTHINGS
    // Nested opt-in (w22 agent2012 / acctB r3019, row 0x006D1250 CTCWife::CheckAppearanceChanges): Ego_r.pdb / FableWin.pdb
    // ?GetTattooThings@CTCHeroMorph@@QBEXAAV?$vector@V?$CDefPointer@$$CBVCThingBaseDef@@@@V?$allocator@...@std@@@std@@@Z (Ego_r 0x00568D50
    // after GetTattoos 0x005686C0 = retail 0x0071DE20 after 0x0071DD50), out of line in retail (`lea eax,<vector> ; push eax ;
    // mov ecx,morph ; call 0x0071DE20`, 0x006D1691). The class block above with this opt-in alone gives the interface key only.
    // Declaration only.
    void GetTattooThings(std::vector<CDefPointer<const CThingBaseDef> >& tattoos) const;   // 0x0071DE20
#endif
};
#else
class CTCHeroMorph;
#endif
#endif // FABLE_ENGINE_CTCHEROMORPH_H
