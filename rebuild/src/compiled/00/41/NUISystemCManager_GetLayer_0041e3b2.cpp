// NUISystem::CManager::GetLayer(long key) -- clamp the key to [-3, 13], look it up in the layer map Layers +0x70 (the tree's _M_find @ 0x0042A879; a hit returns the node's value at +0x14), otherwise return the entry for key 0 (the map's operator[] @ 0x0042D246)
// Leaf: ?GetLayer@CManager@NUISystem@@UAE?AW4EEngine2DLayer@@J@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0041E3B2, length 68 bytes.
// Name (NF227, 2026-11-22, q12582 / q12583): slot 36 of NUISystem::CManager's vtable 0x01230134 (its only reference, .rdata
//   0x012301C4); Ego_r.pdb / FableWin.pdb / Ego_d.pdb ?GetLayer@CManager@NUISystem@@UAE?AW4EEngine2DLayer@@J@Z (public virtual, returns
//   EEngine2DLayer); engine/NUISystem_CManager.h slot 36. Was a row-local model (CManager with a VC6-style CLayerTree stand-in at +0x70)
//   whose non-virtual leaf was ?GetLayer@CManager@NUISystem@@QAEJJ@Z.
// Port: onto engine/NUISystem_CManager.h. Layers is the header's STLport map<long,EEngine2DLayer> (the tree's header node at +0;
//   node value at +0x14). Callees, both out of line in retail: the tree's _M_find 0x0042A879 (FABLE_RETAIL_STLP_TREE_MFIND_DECL; map::find
//   and _Rb_tree::find stay inline, `iterator(_M_find(key))`) and the map's operator[] 0x0042D246 (declared only on the default path of
//   stlport/fable_map.h). The manifest label "std::_Rb_tree::_M_find" (0x0042A879) and the catalog's map<EUsedKeys,long>::operator[]
//   (0x0042D246, the identical-code map<EUsedKeys,long> instance) are the STLport functions bound here.
// Pragmas below the include: the header inlines (e.g. _Rb_tree_base_iterator::operator!= 0x0041C830, 22 B) are then defined in the
//   default /O2 mode, so their pick-any copies equal their owners; above the include the copy is a 21-B frame-pointer body.
// Shape: a size-optimised TU with frame pointers (`push -3 ; pop eax` constants, the clamp stores sharing `mov [ebp+8],eax`), the key
//   slot reused for the 0 key (`and dword ptr [ebp+8],0`), `ret 4`.
#define FABLE_RETAIL_STLP_TREE_MFIND_DECL
#include "engine/NUISystem_CManager.h"
// CManager TU is size-optimised; the pragmas follow the includes (EXTRA_RULES 2026-11-20, nf189 0x005D66BA placement).
#pragma optimize("s", on)
#pragma optimize("y", off)
namespace NUISystem {
EEngine2DLayer CManager::GetLayer(long key)
{
    if (key < -3)
        key = -3;
    else if (key > 13)
        key = 13;
    std::map<long,EEngine2DLayer>::iterator it = Layers.find(key);
    if (it != Layers.end())
        return (*it).second;
    key = 0;
    return Layers[key];
}
}
