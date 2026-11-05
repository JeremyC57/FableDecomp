// IsDistanceFromThingToPositionUnder(const CScriptThing& thing, const C3DVector& position, float distance) -- true when the thing is alive and closer than distance to the position
// Leaf: ?IsDistanceFromThingToPositionUnder@@YI_NABVCScriptThing@@ABVC3DVector@@M@Z
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8).
// Plan phase 4 (2026-11-05), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12). Also: local GetLengthSquared respelled GetSquaredMagnitude (the header / FableWin.pdb method, same computation).
// Retail: Fable.exe (TLC, VC7.1) @ 0x00CBE4B7, 91 bytes (`ret 4`).
// TU: the global quest-script helper functions (__fastcall, size-optimised: `#pragma optimize("s", on)`).
// Model: CScriptThing is polymorphic (vtable 0x01238C8C); the engine header models the vptr as data, so a local
//   virtual model with the retail slot order is used (slots named from Ego_r.pdb / the FSE manifest).
// Identity: Ego_r.pdb `?IsDistanceFromThingToPositionUnder@@YI_NABVCScriptThing@@ABVC3DVector@@M@Z`.
// Callees: CScriptThing slots 75 IsAlive (+0x12C), 6 GetPos (+0x18).
// Shape: 0x00CBE2FF with one thing and a position reference.
#pragma optimize("s", on)
class CCharString {
public:
    void* m_pStringData;
};
#include "engine/C3DVector.h"
#include "engine/CCountedPointer.h"
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"

bool __fastcall IsDistanceFromThingToPositionUnder(const CScriptThing& thing, const C3DVector& position, float distance)
{
    return (thing.IsAlive() && (thing.GetPos() - position).GetSquaredMagnitude() < distance * distance) ? true : false;
}