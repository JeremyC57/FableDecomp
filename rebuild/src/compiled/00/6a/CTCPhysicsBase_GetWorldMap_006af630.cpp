// CTCPhysicsBase::GetWorldMap() const -- the world's map, through the const world
// Leaf: ?GetWorldMap@CTCPhysicsBase@@IBEABVCWorldMap@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x006AF630, 12 bytes: `call 0x00686A80 ; mov edx,[eax] ; mov ecx,eax ; jmp [edx+34h]`.
// Re-identified (agentNF14 2026-11-11; was _unattributed::DisplayEnginePredicate13_006af630, descriptive): Ego_r.pdb
// ?GetWorldMap@CTCPhysicsBase@@IBEABVCWorldMap@@XZ, the const overload right after the non-const one in the Ego_r donor order
// (0x006EC300 / 0x006EC310) as retail 0x006AF620 / 0x006AF630; CWorld vtable slot 13 is PeekWorldMap (engine/CWorld.h
// FABLE_RETAIL_CWORLD); two of its three callers are in CTCPhysicsBase::HasCollidedWithLandscapeAt (0x0088022C / 0x0088024E).
// Callees: CTCBase::PeekWorld() const @ 0x00686A80, CWorld::PeekWorldMap() const (virtual slot 13, tail call).
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_CTCPHYSICSBASE_WORLDMAP
#define FABLE_RETAIL_CWORLD
#include "engine/ETCInterfaceType.h"
#include "engine/CTCPhysicsBase.h"
#include "engine/CWorld.h"

const CWorldMap& CTCPhysicsBase::GetWorldMap() const
{
    return PeekWorld().PeekWorldMap();
}
