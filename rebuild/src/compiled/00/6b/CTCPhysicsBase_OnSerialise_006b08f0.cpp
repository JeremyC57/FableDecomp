// CTCPhysicsBase::OnSerialise(CPersistContext& context, bool) -- persist the position as PositionX/Y/Z relative to the origin of the map it is in; on load restore it (re-adding the map origin for serialise versions above 0), and for a thing flagged 0x20 whose save-game marker is set and lies in the same region, move it to the marker and update its map number
// Grade: RELOCATION_MATCH (565 bytes)
// Leaf: ?OnSerialise@CTCPhysicsBase@@UAEXAAVCPersistContext@@_N@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006B08F0, 565 bytes, `ret 8`; CTCPhysicsBase vtable 0x0125D2C4 slot 1.
// Callees: CTCBase::PeekWorldMap 0x00686D20, CTCBase::PeekWorld 0x00686A80, CTCBase::GetSerialiseVersion 0x00686A90,
//   CPersistContext::Transfer<float> 0x00410620 (through the two-argument Transfer + GetDefaultVal<float>),
//   CWorldMap::GetRegionNumberMapIsIn 0x004FC190, CThing::SetCurrentMapNumber 0x004C7580; CWorldMap::GetMapNumberAt (vslot 16, +0x40)
//   and CWorldMap::GetMapPos (vslot 23, +0x5c, C2DCoordI by hidden pointer). Strings "PositionX/Y/Z" 0x0125D460/54/48;
//   DELTA_NOTIONAL_ZERO 0x0129BA3C (inline C3DVector::IsNonZero); inline CWorld::GetSaveGameMarkerPos (const&, +0x141).
// Flags: none
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCBASE_SERIALISEVERSION
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CWORLD_SAVEGAMEMARKERPOS
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_STATEACCESS
#define FABLE_RETAIL_CPERSISTCONTEXT
#define FABLE_RETAIL_CPERSISTCONTEXT_NAMED_DEFAULTS
#define FABLE_RETAIL_CWORLDMAP
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_NONZERO_SQUARED_XYZ
#include "fable_string.h"
#include "engine/C3DVector.h"
#include "engine/C2DCoordI.h"
#include "engine/CThing.h"
#include "engine/CTCBase.h"
#include "engine/CTCPhysicsBase.h"
#include "engine/CPersistContext.h"
#include "engine/CWorldMap.h"
#include "engine/CWorld.h"
// GetDefaultVal<float> is inline in retail: the two-argument Transfer stores its default 0.0f as `mov dword ptr [esp+0x34],0`
// right before each `call 0x410620` (0x006B097F / 0x006B099D / 0x006B09BB). Same specialisation, copied from the landed rows
// 00/b1/NParticleEngineCPSCEmitterGeneric_DatabaseSerialise_00b13530.cpp and 00/76/NShapeCShape_OnSerialise_00761c00.cpp.
template<> inline float FABLE_FASTCALL CPersistContext::GetDefaultVal<float>() { return 0.0f; }
void CTCPhysicsBase::OnSerialise(CPersistContext& context, bool)
{
    C3DVector pos;
    if (context.IsSaving()) {
        pos = Position;
        C2DCoordI origin = PeekWorldMap().GetMapPos(PeekWorldMap().GetMapNumberAt(pos));
        pos.X -= origin.X;
        pos.Y -= origin.Y;
    }
    context.Transfer("PositionX", pos.X);
    context.Transfer("PositionY", pos.Y);
    context.Transfer("PositionZ", pos.Z);
    if (!context.IsSaving()) {
        Position = pos;
        if (GetSerialiseVersion() > 0) {
            C2DCoordI origin = PeekWorldMap().GetMapPos(ParentThing->GetCurrentMapNumber());
            Position.X += origin.X;
            Position.Y += origin.Y;
        }
        if (ParentThing->GetTypeFlags() & 0x20) {
            if (PeekWorld().GetSaveGameMarkerPos().IsNonZero()) {
                C3DVector marker = PeekWorld().GetSaveGameMarkerPos();
                long markerRegion = PeekWorldMap().GetRegionNumberMapIsIn(PeekWorldMap().GetMapNumberAt(marker));
                const CThing& thing = GetParentThing();
                if (markerRegion == PeekWorldMap().GetRegionNumberMapIsIn(thing.GetCurrentMapNumber())) {
                    Position = marker;
                    GetParentThing().SetCurrentMapNumber(PeekWorldMap().GetMapNumberAt(Position));
                }
            }
        }
    }
}