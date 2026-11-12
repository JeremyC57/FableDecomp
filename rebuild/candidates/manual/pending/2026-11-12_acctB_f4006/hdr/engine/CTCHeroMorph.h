#ifndef FABLE_ENGINE_CTCHEROMORPH_H
#define FABLE_ENGINE_CTCHEROMORPH_H
// CTCHeroMorph (new, chunk 4006; rows 0x005D1F0C / 0x005CE0E6 CTCInventoryStats::ConstructPersonalityListPC / ConstructPersonalityList):
// the hero's appearance-morph thing component as the stats screen reaches it. Declarations only (members opaque; Ego_r.pdb sizeof 0x7C
// with Dinkumware containers, so no size is asserted here).
// Evidence: Ego_r.pdb / FableWin.pdb ?GetTattooNames@CTCHeroMorph@@QAEXAAV?$vector@VCWideString@@V?$allocator@VCWideString@@@std@@@std@@@Z
// (non-const, out of line at 0x0071CE80, landed); FableWin.pdb ??$GetTC@VCTCHeroMorph@@@CThing@@QAE_NPAPAVCTCHeroMorph@@@Z with the
// key TCI_APPEARANCE_MORPH = 3 (`push 3 ; pop edi`, 0x005D2853) from the inherited
// ?StatGetInterfaceType@CTCAppearanceMorphBase@@SI?AW4ETCInterfaceType@@XZ (CTCHeroMorph has no StatGetInterfaceType of its own in
// either PDB; its first Ego_r member PMeshMorphSet is a CCountedPointer<CTCAppearanceMorphBase::CMeshMorphSet>, i.e. the base's).
// Intermediate bases between CTCAppearanceMorphBase and CTCHeroMorph, if any, are not modelled.
#ifndef FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCBASE
#endif
#include "engine/CTCBase.h"
#include "engine/ETCInterfaceType.h"
#include "stlport/fable_containers.h"
class CWideString;
class CTCAppearanceMorphBase : public CTCBase {
public:
    static ETCInterfaceType __fastcall StatGetInterfaceType() { return TCI_APPEARANCE_MORPH; }
};
class CTCHeroMorph : public CTCAppearanceMorphBase {
public:
    void GetTattooNames(std::vector<CWideString>& names);               // 0x0071CE80
};
#endif
