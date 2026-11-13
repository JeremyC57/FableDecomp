// CParticleEnvironmentInteraction::IsParticleCollidingAt(pos, velocity, checkMeshes, hitPos, hitNormal, hitGround) const -- collide a
//   particle step with the world: with checkMeshes, cast pos -> pos + velocity against the physics mesh of every thing (with a physics
//   mesh) in the 4x4 box round pos, keeping the nearest hit (normal reversed); otherwise (or with no mesh hit) test the ground under the
//   start, then under the end of the step (the end case keeps the START's ground height, as retail does); with no world the ground is
//   the plane Z = 0.
// Leaf: ?IsParticleCollidingAt@CParticleEnvironmentInteraction@@UBE_NABVC3DVector@@0_NAAV2@2AA_N@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x0049BE90, 808 bytes, `ret 18h`; CParticleEnvironmentInteraction vtable 0x0122ED8C slot 1.
//   Name from both PDBs (Ego_r 0x004A6240, FableWin 0x01853B2B) and the base NParticleEngine::CAEnvironmentInteraction slot 1.
// Callees: __ftol2 0x00BFEA70 (x2); CThingSearchTools::GetAllThingsInRoughArea<const CThing, vector<const CThing*>,
//   CThingFilter_HasPhysicsMesh> 0x0049C490 (instantiated in this TU, body visible: FABLE_RETAIL_CTHINGSEARCHTOOLS_ROUGHAREABODY);
//   CTCPhysicsBase::GetPhysicsMeshObject 0x006B0850; CPhysicsMeshObject::GetPointIntersectionFromTo 0x00A12450;
//   ~CPhysicsMeshObject 0x00A127D0; free 0x00BFEA14 (vector storage); CWorld::PeekWorldMap (virtual slot 13, +0x34, x3);
//   CWorldMap::GetGroundSizeZAt 0x0063A300 (x2); CWorldMap::GetGroundFaceNormalAt 0x0063AB10. Data: CMainGameComponent::
//   PMainGameComponent 0x013B86A0 (+0x24 world, +0x20 search tools), 0.0f 0x0122DEDC.
// Layout: CParticleEnvironmentInteraction has no engine header: Ego_r.pdb CWorld* PWorld +4 over the shared base header.
// Shape: the ground cases are two separate ifs, each storing hitPos field by field, fetching the face normal and returning (VC7.1
//   tail-merges the second into the first, and the doubled hitNormal use is what puts hitPos/hitNormal in edi/ebp for the whole
//   world path); the mesh hit is tested with nested ifs (no named bool: the call's result is spilled to a frame byte).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT_GETWORLD
#define FABLE_RETAIL_CMAINGAMECOMPONENT_WORLD
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_C2DBOXI_RADIUS_INITIALIZERS
#define FABLE_RETAIL_CTHINGSEARCHTOOLS
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_ROUGHAREABODY
#define FABLE_RETAIL_CTHING_STATTYPEFLAGS
#define FABLE_RETAIL_CTCMAPWHO
#define FABLE_RETAIL_CTCMAPWHO_SEARCHID
#define FABLE_RETAIL_CPHYSICSMESHOBJECT
#define FABLE_RETAIL_CWORLDMAP
#define FABLE_RETAIL_CWORLDMAP_GROUNDFACENORMAL
#include "stlport/fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/C3DVector.h"
#include "engine/C2DCoordI.h"
#include "engine/C2DBoxI.h"
#include "engine/CThing.h"
#include "engine/CTCBase.h"
#include "engine/CTCPhysicsBase.h"
#include "engine/CMainGameComponent.h"
#include "engine/CWorld.h"
#include "engine/CWorldMap.h"
#include "engine/CPhysicsMeshObject.h"
#include "engine/CMeshIntersectionData.h"
class CTCMapwho;
#include "engine/CTCMapwho.h"
#include "engine/NThingMap_CThingMap.h"
#include "engine/CThingSearchTools.h"
#include "engine/CAEnvironmentInteraction.h"

class CThingFilter_HasPhysicsMesh {
public:
    CThingFilter_HasPhysicsMesh() { }
    bool operator()(const CThing& thing) const;             // 0x006A6F50
};

class CParticleEnvironmentInteraction : public NParticleEngine::CAEnvironmentInteraction {
public:
    CWorld* PWorld;                                         // +0x04
    virtual bool IsParticleCollidingAt(const C3DVector& pos, const C3DVector& velocity, const bool checkMeshes, C3DVector& hitPos, C3DVector& hitNormal, bool& hitGround) const;
};

bool CParticleEnvironmentInteraction::IsParticleCollidingAt(const C3DVector& pos, const C3DVector& velocity, const bool checkMeshes, C3DVector& hitPos, C3DVector& hitNormal, bool& hitGround) const
{
    if (PWorld) {
        C3DVector end = pos + velocity;
        if (checkMeshes) {
            std::vector<const CThing*> things;
            CMainGameComponent::Get()->GetWorld().GetThingSearchTools().GetAllThingsInRoughArea<const CThing>(C2DBoxI(pos.ToC2DCoordI(), 2), CThingFilter_HasPhysicsMesh(), things);
            float nearest = 3.402823466e+38F;
            bool found = false;
            for (std::vector<const CThing*>::iterator it = things.begin(); it != things.end(); ++it) {
                CMeshIntersectionData data;
                if ((*it)->GetPhysicsTC()->GetPhysicsMeshObject().GetPointIntersectionFromTo(pos, end, data))
                if (data.DistanceSquared < nearest) {
                    nearest = data.DistanceSquared;
                    hitPos = data.IntersectionPosition;
                    hitNormal = -data.Normal;
                    found = true;
                }
            }
            if (found) {
                hitGround = false;
                return true;
            }
        }
        float groundZ = PWorld->PeekWorldMap().GetGroundSizeZAt(pos);
        float endGroundZ = PWorld->PeekWorldMap().GetGroundSizeZAt(end);
        if (groundZ >= pos.Z) {
            hitGround = true;
            hitPos.X = pos.X;
            hitPos.Y = pos.Y;
            hitPos.Z = groundZ;
            hitNormal = PWorld->PeekWorldMap().GetGroundFaceNormalAt(hitPos);
            return true;
        }
        if (endGroundZ >= end.Z) {
            hitGround = true;
            hitPos.X = end.X;
            hitPos.Y = end.Y;
            hitPos.Z = groundZ;
            hitNormal = PWorld->PeekWorldMap().GetGroundFaceNormalAt(hitPos);
            return true;
        }
        hitGround = false;
        return false;
    }
    if (0.0f > pos.Z) {
        hitPos = pos;
        hitPos.Z = 0.0f;
        hitNormal = C3DVector(0.0f, 0.0f, 1.0f);
        hitGround = true;
        return true;
    }
    hitGround = false;
    return false;
}