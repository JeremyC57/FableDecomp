#ifndef FABLE_ENGINE_CTCDIGGINGSPOT_H
#define FABLE_ENGINE_CTCDIGGINGSPOT_H
// New header (w22 agent2057 scout, row 0x007CACB0 CTCHeroReceiveItems::GlobalUpdate): the digging-spot thing component on the
// polymorphic CTCBase (define FABLE_RETAIL_CTCBASE). Layout: Ego_r.pdb CTCDiggingSpot, 16 bytes (struct_layouts_egor.tsv: Enabled
// +0x0C, Hidden +0x0D). Retail GlobalUpdate stores the flag at [spot+0Ch] (0x007CAEE0 `mov byte ptr [eax+0Ch],0`).
// TCI_DIGGING_SPOT 239: FableWin.pdb ?StatGetInterfaceType@CTCDiggingSpot@@SI?AW4ETCInterfaceType@@XZ (0x0272A7D0) + the row's GetTC
// key 0EFh and HasTC bit (word +0x3C, bit 15: `test ch,ch ; jns`). FableWin.pdb inline ?SetAsEnabled@CTCDiggingSpot@@QAEX_N@Z
// (0x027290D0, 22 bytes / 10 instructions, first COMDAT copy right after GlobalUpdate 0x02726CD0), ?IsEnabled@..QBE_NXZ (0x029BC480),
// ?IsHidden@..QBE_NXZ (0x029BC4A0), ?SetAsHidden@..QAEX_N@Z (0x02AA4880); no Ego_r.pdb / Fable.exe copy (expanded in retail).
#include "engine/CTCBase.h"
#include "engine/ETCInterfaceType.h"
class CTCDiggingSpot : public CTCBase {
public:
    static ETCInterfaceType __fastcall StatGetInterfaceType() { return TCI_DIGGING_SPOT; }
    void SetAsEnabled(bool enabled) { Enabled = enabled; }
    bool IsEnabled() const { return Enabled; }
    void SetAsHidden(bool hidden) { Hidden = hidden; }
    bool IsHidden() const { return Hidden; }
private:
    bool Enabled;                                                        // +0x0c
    bool Hidden;                                                         // +0x0d
};
FABLE_STATIC_ASSERT(sizeof(CTCDiggingSpot) == 0x10);
#endif
