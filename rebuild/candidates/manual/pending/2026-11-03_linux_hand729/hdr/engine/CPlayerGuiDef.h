#ifndef FABLE_ENGINE_CPLAYERGUIDEF_H
#define FABLE_ENGINE_CPLAYERGUIDEF_H
// Retail model (chunk 729, hand-written, partial): Ego_r.pdb CPlayerGuiDef (0xB50 bytes in the PDB) on the retail CDefClassBase.
// Only MiniMapGraphics is modelled: map<CCharString,long> at retail +0x94C (CTCInventoryMap::CreateMiniMapMarkers 0x00603000:
// `lea esi,[PeekGuiDef()+94Ch]` then _Rb_tree::_M_find 0x0042B467 and `cmp eax,[esi]` against end()); Ego_r.pdb has it at +0x9D0,
// after the earlier STLport containers shrink 4 bytes each. Everything before it stays opaque; no size is asserted.
// Requires FABLE_RETAIL_DEF_CLASS_BASE and FABLE_RETAIL_CCHARSTRING.
#include "engine/CDefClassBase.h"
#include "fable_string.h"
#include "stlport/fable_map.h"
class CPlayerGuiDef : public CDefClassBase {
    unsigned char _pad_after_base[0x94c - sizeof(CDefClassBase)];        // CDefClassBase end .. GameActionValues (opaque)
public:
    std::map<CCharString,long> MiniMapGraphics;                          // +0x94C
};
#endif
