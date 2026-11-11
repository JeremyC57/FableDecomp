#ifndef FABLE_ENGINE_CTCHEROMORPH_H
#define FABLE_ENGINE_CTCHEROMORPH_H
// CTCHeroMorph (new shared header, w22 agent2012, row 0x006D1250 CTCWife::CheckAppearanceChanges): the hero's appearance-morph thing
// component (interface TCI_APPEARANCE_MORPH = 3: the row's `test byte ptr [ebx+20h],8` presence bit and `cmp [esi+eax*8],3` key).
// Methods: Ego_r.pdb / FableWin.pdb ?GetTattooThings@CTCHeroMorph@@QBEXAAV?$vector@V?$CDefPointer@$$CBVCThingBaseDef@@@@V?$allocator@...
// (Ego_r 0x00568D50 after GetTattoos 0x005686C0 = retail 0x0071DE20 after 0x0071DD50, the manifest's CTCHeroMorph run), out of line in
// retail (`lea eax,<vector> ; push eax ; mov ecx,morph ; call 0x0071DE20`, 0x006D1691). Declarations only; the members are not modelled.
// Needs FABLE_RETAIL_CTCBASE and FABLE_RETAIL_DEF_CLASS_BASE (CDefPointer) and the STLport vector.
#include "rebuild_abi.h"
#include "stlport/fable_containers.h"
#include "engine/CTCBase.h"
#include "engine/CDefPointer.h"
#include "engine/ETCInterfaceType.h"
class CThingBaseDef;
class CTCHeroMorph : public CTCBase {
public:
    static ETCInterfaceType __fastcall StatGetInterfaceType() { return TCI_APPEARANCE_MORPH; }
    void GetTattooThings(std::vector<CDefPointer<const CThingBaseDef> >& tattoos) const;   // 0x0071DE20
};
#endif
