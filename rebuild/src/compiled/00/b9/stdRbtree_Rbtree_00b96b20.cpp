// std::_Rb_tree<EEngineRenderLayer, std::pair<EEngineRenderLayer const, CEngineSceneRenderStats>, std::_Select1st<std::pair<EEngineRenderLayer const, CEngineSceneRenderStats> >, std::less<EEngineRenderLayer>, std::allocator<std::pair<EEngineRenderLayer const, CEngineSceneRenderStats> > >::_Rb_tree(const std::less<EEngineRenderLayer>&, const std::allocator<std::pair<EEngineRenderLayer const, CEngineSceneRenderStats> >&) -- STLport 4.6 tree constructor _Rb_tree(const _Compare&, const allocator_type&): allocate the header node, colour it red, root 0, leftmost/rightmost = header
// Leaf: ??0?$_Rb_tree@W4EEngineRenderLayer@@U?$pair@$$CBW4EEngineRenderLayer@@VCEngineSceneRenderStats@@@std@@U?$_Select1st@U?$pair@$$CBW4EEngineRenderLayer@@VCEngineSceneRenderStats@@@std@@@3@U?$less@W4EEngineRenderLayer@@@3@V?$allocator@U?$pair@$$CBW4EEngineRenderLayer@@VCEngineSceneRenderStats@@@std@@@3@@std@@QAE@ABU?$less@W4EEngineRenderLayer@@@1@ABV?$allocator@U?$pair@$$CBW4EEngineRenderLayer@@VCEngineSceneRenderStats@@@std@@@1@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B96B20, 56 bytes.
// Types (proven, agentFAM11, 2026-11-12): map<EEngineRenderLayer, CEngineSceneRenderStats> (agentFAM07 proved this tree for 0x00B97130 map<EEngineRenderLayer, CEngineSceneRenderStats>::map, RENAMES 2026-11-11): CEngineScene::RenderStats (Ego_r.pdb map<EEngineRenderLayer, CEngineSceneRenderStats>): the landed ~CEngineScene 0x00B94B50 calls this tree's _M_erase 0x00B95EE0 and binds it with these types; ??0 / ??1 of this map are in the PDB name sets.
//   This _Rb_tree constructor 0x00B96B20 is the COMDAT right before the tree's ~_Rb_tree 0x00B96B60, which calls that _M_erase 0x00B95EE0.
//   Was the placeholder std::_Rb_tree<CUnprovenKey_00b95ee0,CUnprovenValue_00b95ee0,CUnprovenKeyOfValue_00b95ee0,std::less<CUnprovenKey_00b95ee0>,std::allocator<CUnprovenValue_00b95ee0> >::_Rb_tree (leaf ??0?$_Rb_tree@UCUnprovenKey_00b95ee0@@UCUnprovenValue_00b95ee0@@UCUnprovenKeyOfValue_00b95ee0@@U?$less@UCUnprovenKey_00b95ee0@@@std@@V?$allocator@UCUnprovenValue_00b95ee0@@@5@@std@@QAE@ABU?$less@UCUnprovenKey_00b95ee0@@@1@ABV?$allocator@UCUnprovenValue_00b95ee0@@@1@@Z); name-only change: the same member, the per-instantiation placeholder types replaced by
//   the proven ones.
// Model: the shared stlport/fable_map.h (STLport 4.6 _Rb_tree / map; FABLE_RETAIL_STLP_MAP) and engine/CEnginePrimitiveEnums.h; the explicit instantiation emits
//   the member as its own COMDAT, as retail kept every used inline member (no /OPT:REF).
#define FABLE_RETAIL_STLP_MAP
class CEngineSceneRenderStats {   // no shared header for CEngineSceneRenderStats; Ego_r.pdb members (0x1C)
public:
    unsigned long StaticPrimitivesRendered;       // +0x00
    unsigned long MovingPrimitivesRendered;       // +0x04
    unsigned long StaticPrimitivesTouched;        // +0x08
    unsigned long MovingPrimitivesTouched;        // +0x0C
    unsigned long VisibleCells;                   // +0x10
    unsigned long BoundingBoxTests;               // +0x14
    unsigned long BoundingSphereTests;            // +0x18
};
#include "stlport/fable_map.h"
#include "engine/CEnginePrimitiveEnums.h"

template std::_Rb_tree<EEngineRenderLayer, std::pair<EEngineRenderLayer const, CEngineSceneRenderStats>, std::_Select1st<std::pair<EEngineRenderLayer const, CEngineSceneRenderStats> >, std::less<EEngineRenderLayer>, std::allocator<std::pair<EEngineRenderLayer const, CEngineSceneRenderStats> > >::_Rb_tree(const std::less<EEngineRenderLayer>&, const std::allocator<std::pair<EEngineRenderLayer const, CEngineSceneRenderStats> >&);
