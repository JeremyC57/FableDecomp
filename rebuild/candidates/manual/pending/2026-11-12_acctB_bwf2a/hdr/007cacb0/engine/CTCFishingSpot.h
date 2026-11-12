#ifndef FABLE_ENGINE_CTCFISHINGSPOT_H
#define FABLE_ENGINE_CTCFISHINGSPOT_H
// New header (w22 agent2057 scout, row 0x007CACB0 CTCHeroReceiveItems::GlobalUpdate): the fishing-spot thing component on the
// polymorphic CTCBase (define FABLE_RETAIL_CTCBASE and FABLE_RETAIL_CINTELLIGENTPOINTER). Layout: Ego_r.pdb CTCFishingSpot, 40 bytes
// (struct_layouts_egor.tsv: PParticle1 / PParticle2 / PParticleFish CIntelligentPointer<CThing> +0x0C / +0x14 / +0x1C, Enabled +0x24;
// no container member, so retail = Ego_r). Retail GlobalUpdate stores the flag at [spot+24h] (0x007CAEA1 `mov byte ptr [eax+24h],0`).
// TCI_FISHING_SPOT 176: FableWin.pdb ?StatGetInterfaceType@CTCFishingSpot@@SI?AW4ETCInterfaceType@@XZ (0x026D4420) + the row's GetTC
// key 0B0h and HasTC bit (word +0x34, bit 16). FableWin.pdb inline ?SetAsEnabled@CTCFishingSpot@@QAEX_N@Z (0x027290F0, 22 bytes /
// 10 instructions, its first COMDAT copy follows GlobalUpdate 0x02726CD0) and ?IsEnabled@CTCFishingSpot@@QBE_NXZ (0x02A6CEF0); no
// Ego_r.pdb / Fable.exe copy (expanded in retail). The virtual overrides are not declared (vtable not needed by the row).
#include "engine/CTCBase.h"
#include "engine/ETCInterfaceType.h"
#include "engine/CIntelligentPointer.h"
class CThing;
class CTCFishingSpot : public CTCBase {
public:
    static ETCInterfaceType __fastcall StatGetInterfaceType() { return TCI_FISHING_SPOT; }
    void SetAsEnabled(bool enabled) { Enabled = enabled; }
    bool IsEnabled() const { return Enabled; }
private:
    CIntelligentPointer<CThing> PParticle1;                              // +0x0c
    CIntelligentPointer<CThing> PParticle2;                              // +0x14
    CIntelligentPointer<CThing> PParticleFish;                           // +0x1c
    bool Enabled;                                                        // +0x24
};
FABLE_STATIC_ASSERT(sizeof(CTCFishingSpot) == 0x28);
#endif
