// NUISystem::CManager::SetMetaLayer(long metaLayer) -- clamp the meta layer to [0, 4], store it (MetaLayer +0x7c) and rebuild the UI-layer -> engine 2D layer map (Layers +0x70): meta layer 0 gets the fixed table 0x40..0x51 for keys -3..13, any other meta layer maps keys -3..7 onto a run starting at the per-meta-layer base
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0041E1CD, length 485 bytes. Frame-pointer size TU: `#pragma optimize("y", off)` + `("s", on)`.
// Callees: the Layers map's operator[] @ 0x0042D246 (18 calls). Data: the per-meta-layer base table @ 0x01230118.
// Leaf: ?SetMetaLayer@CManager@NUISystem@@UAEXJ@Z
// Names (NF226, 2026-11-22, q12567 unit): virtual, ?SetMetaLayer@CManager@NUISystem@@UAEXJ@Z in Ego_r.pdb (0x0042FC82) and FableWin.pdb
//   (0x01B0E22B), slot 34 of the retail vtable 0x01230134 (= 0x0041E1CD; RenderErrorMessage 0x0041D03C calls it through +0x88);
//   engine/NUISystem_CManager.h `virtual void SetMetaLayer(long)`.
// Port (NF228, 2026-11-22, q12591 / q12601): onto engine/NUISystem_CManager.h (Layers +0x70 = the header's STLport
//   map<long,EEngine2DLayer>, MetaLayer +0x7c, as the landed GetLayer 0x0041E3B2). Was a row-local CManager model with a CLayerTree
//   stand-in whose noinline operator[] bound 0x0042D246 as ??ACLayerTree@@QAEAAW4EEngine2DLayer@@ABJ@Z; the 18 calls now bind the
//   STLport ??A?$map@JW4EEngine2DLayer@@U?$less@J@std@@V?$allocator@...@3@@std@@QAEAAW4EEngine2DLayer@@ABJ@Z that GetLayer binds.
// Shape: the clamp constants 4 and 5 are hoisted into edi/ebx (`push 4 ; pop edi`) and reused as the keys 4 and 5; every key is a
//   const-ref temporary homed in the dead argument slot [ebp+8]. The map operator[] body is visible in the retail TU (same-TU
//   template instance, out of line), and so is the tree's _M_lower_bound (FABLE_RETAIL_STLP_TREE_BOUNDS_OUTOFLINE +
//   FABLE_RETAIL_STLP_TREE_BOUNDS_BODY_PASS22): VC7.1 then knows the key's address does not escape and hoists the `or [ebp+8],-1` /
//   `and [ebp+8],0` key stores above the previous value store (with _M_lower_bound declared only, those two stores follow it: DIFFER).
// Pick-any copies (NF228 pk.py, relocations alike): operator[] = 0x0042D246, map::insert = 0x0042CE46, _M_lower_bound = 0x0042AE33,
//   _Rb_tree_base_iterator::operator== = 0x0041C820; the rest have no retail owner. They need the row-tail `optimize("y", on)` below.
// Pragmas below the include (EXTRA_RULES 2026-11-20, nf189 placement).
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_STLP_TREE_BOUNDS_OUTOFLINE
#define FABLE_RETAIL_STLP_TREE_BOUNDS_BODY_PASS22
#include "engine/NUISystem_CManager.h"
#pragma optimize("y", off)
#pragma optimize("s", on)
extern long g_metaLayerBase[5];                    // 0x01230118
namespace NUISystem {
void CManager::SetMetaLayer(long metaLayer)
{
    if (metaLayer < 0)
        metaLayer = 0;
    else if ((unsigned long)metaLayer >= 5)
        metaLayer = 4;
    MetaLayer = metaLayer;
    if (metaLayer == 0) {
        Layers[-3] = (EEngine2DLayer)0x40;
        Layers[-2] = (EEngine2DLayer)0x41;
        Layers[-1] = (EEngine2DLayer)0x42;
        Layers[0] = (EEngine2DLayer)0x43;
        Layers[1] = (EEngine2DLayer)0x44;
        Layers[2] = (EEngine2DLayer)0x45;
        Layers[3] = (EEngine2DLayer)0x46;
        Layers[4] = (EEngine2DLayer)0x47;
        Layers[5] = (EEngine2DLayer)0x49;
        Layers[6] = (EEngine2DLayer)0x4a;
        Layers[7] = (EEngine2DLayer)0x4b;
        Layers[8] = (EEngine2DLayer)0x4c;
        Layers[9] = (EEngine2DLayer)0x4d;
        Layers[10] = (EEngine2DLayer)0x4e;
        Layers[11] = (EEngine2DLayer)0x4f;
        Layers[12] = (EEngine2DLayer)0x50;
        Layers[13] = (EEngine2DLayer)0x51;
    } else {
        long base = g_metaLayerBase[metaLayer];
        for (long i = -3; i <= 7; i++) {
            long key = i;
            Layers[key] = (EEngine2DLayer)(base + i + 2);
        }
    }
}
}
// Fidelity: scaffolding, not retail. Restores the TU default /Oy after the function, so the pick-any template COMDATs below
// (operator[], map::insert, _M_lower_bound) take the s-on /Oy mode their owners 0x0042D246 / 0x0042CE46 / 0x0042AE33 have.
#pragma optimize("y", on)
