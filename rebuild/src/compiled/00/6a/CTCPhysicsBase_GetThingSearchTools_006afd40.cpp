// CTCPhysicsBase::GetThingSearchTools() const -- the world's thing-search tools, for the physics thing components
// Leaf: ?GetThingSearchTools@CTCPhysicsBase@@IBEABVCThingSearchTools@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x006AFD40, 9 bytes: `call 0x00686A80 ; mov eax,[eax+20h] ; ret`.
// Identity (respelled 2026-11-16, landing gl; was CTCBase::PeekThingSearchTools, which is 0x00686D90): Ego_r.pdb
//   ?GetThingSearchTools@CTCPhysicsBase@@IBEABVCThingSearchTools@@XZ 0x006EC500 / FableWin.pdb 0x0246E010, in the Ego_r order
//   IsPhysicsMeshTypeMesh / GetThingSearchTools / FrameUpdate = retail 0x006AFD30 / 0x006AFD40 / 0x006AFD60; all four callers are
//   CTCPhysics* methods (0x00725B70, 0x00726BE0, 0x0075AA80, 0x008E57D0). The body is `*PeekWorld().PThingSearchTools` (CWorld +0x20
//   scoped_ptr<CThingSearchTools>, Ego_r.pdb and the landed engine/CWorld.h) through CTCBase::PeekWorld 0x00686A80 and the inline CWorld::PeekThingSearchTools.
// Header: engine/CTCPhysicsBase.h (FABLE_RETAIL_CTCPHYSICSBASE_GETTHINGSEARCHTOOLS), engine/CTCBase.h, engine/CWorld.h (FABLE_RETAIL_CWORLD).
class CThingSearchTools;
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_CTCPHYSICSBASE_GETTHINGSEARCHTOOLS
#define FABLE_RETAIL_CWORLD
#include "engine/CTCPhysicsBase.h"
#include "engine/CWorld.h"

const CThingSearchTools& CTCPhysicsBase::GetThingSearchTools() const
{
    return PeekWorld().PeekThingSearchTools();
}
