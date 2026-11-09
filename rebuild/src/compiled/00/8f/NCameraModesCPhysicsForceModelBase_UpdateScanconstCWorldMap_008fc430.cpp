// NCameraModes::CPhysicsForceModelBase::UpdateScan<const CWorldMap>(CPhysicsCollisionPointsObject& points, const CWorldMap& map, const C3DVector& centre, long flags, float radius) -- one frame of the rolling collision scan: for each due latitude ring, drop the oldest sample when full and append the sphere point at this frame's longitude
// Leaf: ??$UpdateScan@$$CBVCWorldMap@@@CPhysicsForceModelBase@NCameraModes@@IAEXAAVCPhysicsCollisionPointsObject@1@ABVCWorldMap@@ABVC3DVector@@JM@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008FC430, length 876 bytes, thiscall, `ret 14h`.
// Identity: PDB ??$UpdateScan@$$CBVCWorldMap@@@CPhysicsForceModelBase@NCameraModes@@IAEXAAVCPhysicsCollisionPointsObject@1@
//   ABVCWorldMap@@ABVC3DVector@@JM@Z; called from the landed DoGroundEvasion 0x008FADB0 and GetCollisionPoints<const CWorldMap>
//   0x008FC810. Catalog "ComputeEvasionParticleTrajectory" is descriptive. The world-map instance never reads the map or
//   the flags (no intersection call in retail: the sphere point itself is appended).
// Callees: CWorld::GetFrame 0x0049D870 (static __fastcall), __ftol2 0x00BFEA70, free 0x00BFEA14 (list node), malloc
//   0x00BFEA0E (list node). Constants: DELTA_NOTIONAL_ZERO 0x0129BA3C (1e-4f, not folded), 0.5f 0x0122F59C, 0.25f 0x0123078C,
//   14.3f 0x01288C34, 1.0f 0x0122DED8, 2*pi (double of the float) 0x0124F2B8; the X/Y/Z axis vectors at 0x013893B8 /
//   0x013893C4 / 0x013893D0 (no PDB data names; descriptive).
// Layout (retail): engine/CPhysicsCollisionPointsObject.h (0x24 bytes); CPhysicsForceModelBase::TriggerFrame +0x9C (PDB
//   +0xE4; local model -- the shared NCameraModes_CPhysicsLeaf.h declares only its virtuals: header debt).
// Scheduling (chunk 892): c2 cuts a basic block into list-scheduler regions of at most 81 tuples (c2.dll 0x107B4E6A
//   `cmp esi,50h ; jg`), and every no-code PAREN tuple (0x269) of a parenthesised floating value counts. Retail's first
//   region of the sphere-point expression ends at `fmul st(2)` (0x008FC6B7), which takes 75 instructions + 6 PARENs: the
//   parenthesised trigonometric factors below (the cosines doubly). One PAREN fewer leaves `fstp [esp+1Ch]; fmul st(1)`
//   in region 1 and the list-end load / `push 14h` (0x008FC6BD) below them; none at all, 5 more x87 ops.
#include "rebuild_abi.h"
#pragma intrinsic(sin, cos)
extern "C" double FABLE_CDECL sin(double);
extern "C" double FABLE_CDECL cos(double);

#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_STLP_LIST
#include "engine/C3DVector.h"
#include "engine/CPhysicsCollisionPointsObject.h"
extern const C3DVector g_XAxis;                                // 0x013893B8
extern const C3DVector g_YAxis;                                // 0x013893C4
extern const C3DVector g_ZAxis;                                // 0x013893D0

#include "engine/CWorld.h"
class CWorldMap;

namespace NCameraModes {

class CPhysicsForceModelBase {
public:
    virtual void v00();
    void*                         LinkedState;                 // +0x04
    CPhysicsCollisionPointsObject Things;                      // +0x08
    CPhysicsCollisionPointsObject Themes;                      // +0x2C
    CPhysicsCollisionPointsObject Ground;                      // +0x50
    C3DVector                     LastAcceleration;            // +0x74
    C3DVector                     ScanCenter;                  // +0x80
    C3DVector                     AverageCamPos;               // +0x8C
    float                         ScanRadSquared;              // +0x98
    long                          TriggerFrame;                // +0x9C

protected:                                                 // Ego_r.pdb / FableWin.pdb IAE (protected)
    template <class T>
    void UpdateScan(CPhysicsCollisionPointsObject& points, T& container, const C3DVector& centre, long flags, float radius);
};

template <class T>
void CPhysicsForceModelBase::UpdateScan(CPhysicsCollisionPointsObject& points, T& container, const C3DVector& centre,
                                        long flags, float radius)
{
    unsigned long frame = CWorld::GetFrame();
    long density = points.ScanDensity;
    long step = frame % density;
    TriggerFrame = frame;
    float phase = (float)step / density;
    if (TriggerFrame < frame + points.ScanDensity)
    {
        for (float theta = points.StartTheta; theta <= DELTA_NOTIONAL_ZERO + 0.5f; theta += points.DeltaTheta)
        {
            long count = (long)((1.0f - (theta - 0.25f) * (theta - 0.25f) * 14.3f) * points.ScanDensity);
            if (frame % (points.ScanDensity / count) != 0)
                continue;

            std::list<std::pair<float, std::list<C3DVector> > >::iterator it = points.SmartPointList.begin();
            bool found = false;
            while (it != points.SmartPointList.end() && !found)
            {
                float ringTheta = (*it++).first;
                if (ringTheta > theta - DELTA_NOTIONAL_ZERO && ringTheta < DELTA_NOTIONAL_ZERO + theta)
                    found = true;
            }
            --it;
            std::list<C3DVector>& ring = (*it).second;
            if (ring.size() == (unsigned long)count)
                ring.pop_front();

            // parenthesised factors: 6 PAREN tuples, retail's region cut at 0x008FC6B7 (see the header note)
            C3DVector end = centre + g_YAxis * radius * ((cos(phase * 6.2831854820251465))) * (sin(theta * 6.2831854820251465))
                                   + g_XAxis * radius * (sin(phase * 6.2831854820251465)) * (sin(theta * 6.2831854820251465))
                                   + g_ZAxis * radius * ((cos(theta * 6.2831854820251465)));
            ring.push_back(end);
        }
    }
}

template void CPhysicsForceModelBase::UpdateScan<const CWorldMap>(CPhysicsCollisionPointsObject&, const CWorldMap&,
                                                                  const C3DVector&, long, float);

}   // namespace NCameraModes