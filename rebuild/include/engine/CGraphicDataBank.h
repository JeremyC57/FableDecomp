#ifndef FABLE_ENGINE_CGRAPHICDATABANK_H
#define FABLE_ENGINE_CGRAPHICDATABANK_H
// New header (chunk 767, row 0x00B2AD80 CEngineSurfaceCompositionManager::ChangeSceneForTextureBank). Retail RTTI
// .?AVCGraphicDataBank@@ : CBankFileAsync (+0x0), CResourceBank (+0x164) (ghidra_out/rtti_bases.tsv); sizeof 0x30C (the surface
// manager's three banks sit at +0x8C / +0x398 / +0x6A4, engine/CEngineSurfaceCompositionManager.h). The bases and members are not
// modelled yet (one opaque block); only the members rows call are declared. Ego_r.pdb / FableWin.pdb ?ChangeScene@CGraphicDataBank@@QAEXXZ,
// retail 0x009FD0C0 (landed).
#include "rebuild_abi.h"
class CGraphicDataBank {
public:
    void ChangeScene();                                      // 0x009FD0C0
private:
    unsigned char Unmodeled_0x000[0x30c];                    // CBankFileAsync +0x0, CResourceBank +0x164, own members (not modelled)
};
FABLE_STATIC_ASSERT(sizeof(CGraphicDataBank) == 0x30c);
#endif // FABLE_ENGINE_CGRAPHICDATABANK_H
