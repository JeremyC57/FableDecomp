// CThingTrackNode::DrawGetDrawSizeX(float, float*) const -- writes the node's draw size along x (the thing's Scale +0x9C times the global definition's TrackGraphic RenderSizeX, CGlobalDef +0xE6 +0x08) to the out-parameter; the float argument is unused
// Leaf: ?DrawGetDrawSizeX@CThingTrackNode@@QBEXMPAM@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00837060, length 35 bytes (`ret 8`: a float and the out pointer).
// Callees: ?PeekGlobalDefinition@CThing@@IBEABVCGlobalDef@@XZ @ 0x004C79A0 (protected const).
// Shape: `this` is kept in esi across the call; the dead `add eax,0E6h` next to `fmul [eax+0EEh]` is a named POINTER local to the
// track graphic (the direct form folds the offset and drops the add); `graphic->RenderSizeX * Scale` loads the scale first.
// NF63 (round 7): moved onto engine/CThingTrackNode.h (CThingGameObject -> CThingPhysical -> CThing; Scale is CThingPhysical's
// protected +0x9C) and engine/CGlobalDef.h / CEngineGraphic.h; self ?DrawGetDrawSizeX@CThingTrackNode@@QBEXMPAM@Z (Ego_r.pdb 0x0082ABB0 +
// FableWin.pdb 0x027FD340: const, was QAE in a local no-base model); 0x004C79A0 was a free cdecl GetMeshBank() stand-in.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_PEEKGLOBALDEFINITION
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGTRACKNODE
#define FABLE_RETAIL_CTHINGTRACKNODE_DRAW
#include "fable_string.h"
#include "engine/CGlobalDef.h"
#include "engine/CThingTrackNode.h"
void CThingTrackNode::DrawGetDrawSizeX(float unused, float* out) const
{
    const CEngineGraphic* graphic = &PeekGlobalDefinition().TrackGraphic;
    *out = graphic->RenderSizeX * Scale;
}
