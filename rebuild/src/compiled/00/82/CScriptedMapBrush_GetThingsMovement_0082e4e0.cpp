// CScriptedMapBrush::GetThingsMovement(unsigned __int64 uid) const -- the recorded position / facing of a thing the brush moves: the ThingsToMove map's value for uid (no end test)
// Leaf: ?GetThingsMovement@CScriptedMapBrush@@QBEABVCThingPosAndOrientation@1@_K@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0082E4E0, 19 bytes (+13 int3), thiscall, `ret 8` (the id by value): `lea eax,[esp+4] ; push eax ;
//   add ecx,20h ; call 0x0082EEB0 ; add eax,18h`: STLport 4.6 const map::find -> _Rb_tree::find -> the out-of-line member-template
//   _M_find (0x0082EEB0, the landed row) on ThingsToMove (+0x20), then ->second (node +0x10 value, +8 key).
// NF175 (2026-11-20, q4390): re-spelled from the stand-in CSurface::GetHeight (local FableSurf_0082e4e0 / FableSurfHelper_0082e4e0, whose
//   ?compute@FableSurfHelper_0082e4e0@@QAEJPAX@Z bound the tree's _M_find 0x0082EEB0) to Ego_r.pdb 0x00735340 + FableWin.pdb 0x027CABD0
//   ?GetThingsMovement@CScriptedMapBrush@@QBEABVCThingPosAndOrientation@1@_K@Z (Ego_r places it directly before RemoveThing 0x00735360, as
//   retail 0x0082E4E0 sits before RemoveThing 0x0082E500); the landed caller 0x00820840 CMap::ScriptedMapBrushMoveThings already binds this
//   spelling. Shared engine/CScriptedMapBrush.h (retail branch, THINGSETS + NF175 THINGSTOMOVE opt-in) and stlport/fable_map.h.
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CSCRIPTEDMAPBRUSH
#define FABLE_RETAIL_CSCRIPTEDMAPBRUSH_THINGSETS
#define FABLE_RETAIL_CSCRIPTEDMAPBRUSH_THINGSTOMOVE
#define FABLE_RETAIL_STLP_TREE_CONSTFIND
#define FABLE_RETAIL_STLP_TREE_MFIND_DECL
#include "stlport/fable_map.h"
#include "engine/CScriptedMapBrush.h"

const CScriptedMapBrush::CThingPosAndOrientation& CScriptedMapBrush::GetThingsMovement(unsigned __int64 uid) const
{
    return ThingsToMove.find(uid)->second;
}
