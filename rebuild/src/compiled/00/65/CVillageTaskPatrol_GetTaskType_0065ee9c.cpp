// Leaf: ?GetTaskType@CVillageTaskPatrol@@UBE?AW4EVillageTaskType@@XZ
// Switch route (NF318, 2026-11-23): defined on the real class of engine/CVillageTaskPatrol.h (class-model switch FABLE_RETAIL_CVILLAGETASKPATROL; was ?GetTaskType@CVillageTaskPatrol_Methods@@QAEHXZ);
//   the retail RTTI vftable placement (ghidra_out/labels_rtti_port.tsv) names this PDB member at this address. Bytes unchanged.
// Preamble: the define / include block of the landed precedent CVillageTaskPatrol_IsCreatureMoreAppropriateForTask_00860ad0.cpp.
#define FABLE_RETAIL_CTHINGTRACKNODE_PEEK_NEXT
#define FABLE_RETAIL_CVILLAGETASKPATROL_GET_START
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CVILLAGETASKPATROL
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGTRACKNODE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_GET_ORIGINAL_CHAIN
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_NORMALISE_SQUARE_HELPER
#define FABLE_RETAIL_C3DVECTOR_SQUAREDMAGNITUDE_XYZ
#include "engine/CVillageTaskPatrol.h"
#include "engine/CThingTrackNode.h"
#pragma optimize("s",on)
// Constant-return member: returns 1. __fastcall this=ecx.
EVillageTaskType CVillageTaskPatrol::GetTaskType() const { return (EVillageTaskType)1; }
