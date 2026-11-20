// CScriptedMapBrush::SetThingToDelete(unsigned __int64 uid) -- add the thing's unique id to the brush's ThingsToDelete set (the insert's result is dropped)
// Leaf: ?SetThingToDelete@CScriptedMapBrush@@QAEX_K@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0082E8F0, thiscall, `ret 8` (the id by value). One 8-byte local (the dropped pair<iterator,bool>),
//   then std::set<unsigned __int64>::insert's out-of-line _Rb_tree::insert_unique(const value_type&) 0x0082F4B0 on this+0x14.
// NF123 (2026-11-20, q4518): re-spelled from the stand-in CTCLook::IsTurningBody (Vec2 / LookSub local models) to Ego_r.pdb 0x007353F0 + FableWin.pdb
//   0x027CB060 ?SetThingToDelete@CScriptedMapBrush@@QAEX_K@Z, on the shared engine/CScriptedMapBrush.h (retail branch, NF123 THINGSETS opt-in: the
//   member is Ego_r's set<unsigned __int64>) and stlport/fable_map.h; the landed caller 0x00821010 CMap::LoadBrushesFromFile binds it.
//   The callee 0x0082F4B0 now binds the STLport insert_unique spelling (was the stand-ins ?Compare@LookSub@@QAE_NPAUVec2@@0@Z / ?Query@SubObj@@QAE_NPAUVec8@@0@Z).
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CSCRIPTEDMAPBRUSH
#define FABLE_RETAIL_CSCRIPTEDMAPBRUSH_THINGSETS
#include "stlport/fable_map.h"
#include "engine/CScriptedMapBrush.h"

void CScriptedMapBrush::SetThingToDelete(unsigned __int64 uid)
{
    ThingsToDelete.insert(uid);
}
