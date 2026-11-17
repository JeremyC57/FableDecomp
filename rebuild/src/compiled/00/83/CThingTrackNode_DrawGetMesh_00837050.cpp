// CThingTrackNode::DrawGetMesh() const -- the mesh bank index of the global definition's track graphic (CGlobalDef::TrackGraphic +0xE6, BankIndex +0)
// Leaf: ?DrawGetMesh@CThingTrackNode@@QBEKXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00837050, length 12 bytes. No callers and no vtable slot (it follows
// DrawIsDrawable @ 0x00837040 in the same catalogue row, split via the skipped-row ledger, chunk 905).
// Callees: ?PeekGlobalDefinition@CThing@@IBEABVCGlobalDef@@XZ @ 0x004C79A0 (protected const; ecx = this, ignored by the callee).
// Shape: `call ; mov eax,[eax+0E6h] ; ret` -- CGlobalDef is byte-packed (+0xE6 is not dword aligned).
// NF63 (round 7): moved onto engine/CThingTrackNode.h (CThingGameObject -> CThingPhysical -> CThing) and engine/CGlobalDef.h /
// CEngineGraphic.h; the self is ?DrawGetMesh@CThingTrackNode@@QBEKXZ (Ego_r.pdb 0x0082ABA0 + FableWin.pdb 0x027FD1F0; Ego_r order
// DrawIsDrawable / DrawGetMesh / DrawGetDrawSizeX = retail 0x00837040 / 0x00837050 / 0x00837060), was the made-up GetDrawRecordA over
// local CMeshDataBank / CMeshInfoSize models and a free cdecl GetMeshBank() stand-in for 0x004C79A0.
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
unsigned long CThingTrackNode::DrawGetMesh() const
{
    return PeekGlobalDefinition().TrackGraphic.BankIndex;
}
