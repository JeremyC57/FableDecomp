// CTCPhysicsBase::GetWorldMap() -- the world's map, through the world
// Leaf: ?GetWorldMap@CTCPhysicsBase@@IAEAAVCWorldMap@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x006AF620, 12 bytes: `call 0x00686A70 ; mov edx,[eax] ; mov ecx,eax ; jmp [edx+30h]`.
// Identity (agentNF14 2026-11-11; the catalog name; landed as CThingTrackNode::DrawIsDrawable over a free GetDisplayEngine stub):
// Ego_r.pdb ?GetWorldMap@CTCPhysicsBase@@IAEAAVCWorldMap@@XZ, followed in the Ego_r donor order by the const overload, as retail
// 0x006AF630 (`call PeekWorld 0x00686A80 ; jmp [vtbl+34h]`) follows this one; CWorld vtable slot 12 is GetWorldMap (slot 13
// PeekWorldMap, engine/CWorld.h FABLE_RETAIL_CWORLD).
// Callees: CTCBase::GetWorld() const @ 0x00686A70, CWorld::GetWorldMap() (virtual slot 12, tail call).
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_CTCPHYSICSBASE_WORLDMAP
#define FABLE_RETAIL_CWORLD
#include "engine/ETCInterfaceType.h"
#include "engine/CTCPhysicsBase.h"
#include "engine/CWorld.h"

CWorldMap& CTCPhysicsBase::GetWorldMap()
{
    return GetWorld().GetWorldMap();
}
