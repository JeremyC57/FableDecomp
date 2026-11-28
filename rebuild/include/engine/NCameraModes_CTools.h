// Retail opt-in (Codex pass21): FABLE_RETAIL_CTCTARGETINGPLAYER_UPDATE_LIST exposes original targeting helpers; row00804020 retains both-PDB/donor/native proof.
#ifndef FABLE_ENGINE_NCAMERAMODES_CTOOLS_H
#define FABLE_ENGINE_NCAMERAMODES_CTOOLS_H
// New header (chunk 724 round 3). NCameraModes::CTools, the camera helper owned by CGameCameraManager (Ego_r.pdb
// ??0CTools@NCameraModes@@QAE@ABVCGameCameraManager@@@Z / ?GetCameraTools@CGameCameraManager@@QAEAAVCTools@NCameraModes@@XZ).
// Ego_r TPI size 0x24 with a Dinkumware vector; retail STLport vector is 0xC: CameraManager +0x00, ScanThings +0x04,
// CameraMeshOffsetDistance +0x10, CameraCreatureOffsetDistance +0x14, CameraLandscapeOffsetDistance +0x18,
// CameraThemeOffsetDistance +0x1C (sizeof 0x20; the landed IsLineOfSight 0x00877C40 reads ScanThings at +0x04).
// ELOSTest is the nested enum the PDB mangles as W4ELOSTest@CTools@NCameraModes@@; no row yet needs its enumerators (the
// thing-type test reads bit 3 of it at 0x0087724E).
#include "rebuild_abi.h"
#include "stlport/fable_containers.h"
class C3DVector;
class CThing;
class CGameCameraManager;
#ifdef FABLE_RETAIL_NCAMERAMODES_CTOOLS_WORLD
class CWorld;
#endif
#ifdef FABLE_RETAIL_NCAMERAMODES_CTOOLS_WORLD
#include "engine/CGameCameraManager.h"
#endif
#ifdef FABLE_RETAIL_NCAMERAMODES_CTOOLS_POTENTIALFIELD
#include "engine/C3DVector.h"
class CThingGameObject;
#endif
#ifdef FABLE_RETAIL_NCAMERAMODES_CTOOLS_MESHTYPE
class CPhysicsMeshObject;
#endif
namespace NCameraModes {
#ifdef FABLE_RETAIL_NCAMERAMODES_CTOOLS_MESHTYPE
// Retail opt-in (wf21 row 0x00875B10): Ego_r.pdb / FableWin.pdb
// ?TestMeshForOccupationAndGetType@CTools@NCameraModes@@QBE?BW4EMeshType@2@ABVCPhysicsMeshObject@@ABVC3DVector@@1AAM@Z -- EMeshType is
// a namespace-scope enum of NCameraModes (`W4EMeshType@2@`). Enumerator names found nowhere: descriptive (values 0 / 1).
enum EMeshType { MESH_TYPE_NONE = 0, MESH_TYPE_OCCUPIED = 1 };
#endif
class CTools {
public:
#ifdef FABLE_RETAIL_NCAMERAMODES_CTOOLS_FRUSTRUM_POS
    // Retail opt-in (Codex pass 24): Ego_r 0x0085B8F0 and FableWin 0x029C2520 name this const member.
    // Retail 0x00697F50 and 0x00697F80 call the same body at 0x00875120 with two other positions and Camera.GetFOV().
    bool IsPosInViewingFrustrum(const C3DVector& pos, const C3DVector& look_at, const C3DVector& camera, float fov) const;
#endif
#ifdef FABLE_RETAIL_NCAMERAMODES_CTOOLS_ISPOSINVIEWINGFRUSTRUM
    // Retail opt-in (Codex pass 24): both PDB names and 0x00875120.
    bool IsPosInViewingFrustrum(const C3DVector&, const C3DVector&, const C3DVector&, float) const;
#endif
#ifdef FABLE_RETAIL_NCAMERAMODES_CTOOLS_MESHTYPE
    const EMeshType TestMeshForOccupationAndGetType(const CPhysicsMeshObject& mesh,const C3DVector& position,const C3DVector& up,float& height) const;   // 0x00875B10
#endif
#ifdef FABLE_RETAIL_NCAMERAMODES_CTOOLS_LANDSCAPEFROMTO
    // Ego_r.pdb ?GetLandscapePointIntersectionFromTo@CTools@NCameraModes@@QBE_NABVC3DVector@@0AAV3@1_N@Z (const). Declaration only.
    bool GetLandscapePointIntersectionFromTo(const C3DVector& from,const C3DVector& to,C3DVector& hitPos,C3DVector& hitNormal,bool includeWater) const;   // 0x00875550
#endif
#ifdef FABLE_RETAIL_NCAMERAMODES_CTOOLS_CTOR
    // Retail opt-in (chunk 2033, row 0x0069AE80 CGameCameraManager::CGameCameraManager): Ego_r.pdb / FableWin.pdb
    // ??0CTools@NCameraModes@@QAE@ABVCGameCameraManager@@@Z, out of line in retail at 0x00877160 (the manager's constructor
    // passes its own `this`: `push esi ; lea ecx,[esi+78h] ; call 0x00877160`). Declaration only, no layout change.
    CTools(const CGameCameraManager& camera_manager);                    // 0x00877160
#endif
#ifdef FABLE_RETAIL_NCAMERAMODES_CTOOLS_ROLLING_AVERAGE
    // Retail opt-in (Codex pass 17): original donor template and arithmetic at0x02889720.
#ifdef FABLE_RETAIL_NCAMERAMODES_CTOOLS_ROLLING_OUTOFLINE
    template<class T> T GetRollingAverage(const T& current_val,const T& new_val,float weight)const;
#else
    template<class T> T GetRollingAverage(const T& current_val,const T& new_val,float weight) const
    {return (current_val*weight+new_val)/(weight+1.0f);}
#endif
#endif
#if defined(FABLE_RETAIL_CTCTARGETINGPLAYER_UPDATE_LIST) || defined(FABLE_RETAIL_NCAMERAMODES_CTOOLS_PASS22_LOS) // Retail opt-in (Codex pass22): both original PDB enum/signature; native00877C40.
    enum ELOSTest { TEST_THINGS=1,TEST_LANDSCAPE=2,TEST_THEMES=4,TEST_CREATURES=8,TEST_PASSABILITY=16,TEST_ALL_BUT_CREATURES=7,TEST_ALL=15 };
    bool IsLineOfSight(const C3DVector&,const C3DVector&,C3DVector&,C3DVector&,float&,const CThing**,ELOSTest,bool)const;
#else
    enum ELOSTest { };
#endif
#ifdef FABLE_RETAIL_NCAMERAMODES_CTOOLS_LINEOFSIGHT_PASS24
    // Ego_r PDB and retail call at 0x008754A0.
    bool GetThemePointIntersectionFromTo(const C3DVector&, const C3DVector&, C3DVector&, C3DVector&, bool) const;
#endif
    bool GetThingPointIntersectionFromTo(const std::vector<const CThing*>&, const C3DVector&, const C3DVector&, C3DVector&,
        C3DVector&, const CThing**, bool, ELOSTest) const;           // 0x008771A0
    // chunk 724c: the thing-vector IsLineOfSight overload, Ego_r ?IsLineOfSight@CTools@NCameraModes@@QBE_NABV?$vector@PBVCThing@@...
    // ABVC3DVector@@1AAV5@2AAMPAPBVCThing@@W4ELOSTest@12@_N@Z = retail 0x00877840 (the landed 0x00877C40 calls it).
    bool IsLineOfSight(const std::vector<const CThing*>&, const C3DVector&, const C3DVector&, C3DVector&, C3DVector&, float&,
        const CThing**, ELOSTest, bool) const;                           // 0x00877840
#ifdef FABLE_RETAIL_NCAMERAMODES_CTOOLS
    // chunk 751 (row 0x00878390): Ego_r.pdb / FableWin.pdb member template
    // ??$GetThingsOfType@VCThingBuilding@@@CTools@NCameraModes@@QBEXAAV?$list@PBVCThingBuilding@@...@std@@JABVC3DVector@@M@Z.
    template<class T> void GetThingsOfType(std::list<const T*>& things,long type,const C3DVector& pos,float radius) const;
#ifdef FABLE_RETAIL_NCAMERAMODES_CTOOLS_STANDARDTHINGS
    // Retail opt-in (chunk F1, row 0x00877720): FableWin.pdb / Ego_r.pdb
    // ?GetStandardThings@CTools@NCameraModes@@QBEXAAV?$list@PBVCThing@@V?$allocator@PBVCThing@@@std@@@std@@ABVC3DVector@@M@Z.
    void GetStandardThings(std::list<const CThing*>& things,const C3DVector& pos,float radius) const;
#endif
    // Chunk 756 (row 0x00875F00): Ego_r.pdb ?GetMeshExtent@CTools@NCameraModes@@QBEMABVCThing@@@Z.
    float GetMeshExtent(const CThing& thing) const;
#endif
#ifdef FABLE_RETAIL_NCAMERAMODES_CTOOLS_POTENTIALFIELD
    // Retail opt-in (chunk Q14, row 0x00876CA0): Ego_r.pdb ?PotentialField@CTools@NCameraModes@@QBEMABVC3DVector@@0MM@Z (Ego_r
    // 0x0085BE80; no out-of-line copy in retail: expanded in 0x00876CA0 and the landed 0x00876E10, the body the latter models,
    // parameter names as NCameraModes::CPhysicsForceModelBase's copy) and
    // ?PotentialFieldBufferObjects@CTools@NCameraModes@@QBE?AVC3DVector@@ABV3@ABV?$list@PBVCThingGameObject@@...@std@@@Z (Ego_r
    // 0x0085D8B0; retail 0x00876CA0). No layout change.
    float PotentialField(const C3DVector& pos,const C3DVector& source,float scale,float r_prime) const
    {
        return (scale*0.1f)/((pos-source).GetMagnitude()-r_prime);
    }
    C3DVector PotentialFieldBufferObjects(const C3DVector& field_is_at,const std::list<const CThingGameObject*>& objects) const;   // 0x00876CA0
#endif
private:
#ifdef FABLE_RETAIL_NCAMERAMODES_CTOOLS_WORLD
    const CGameCameraManager& GetCameraManager()const { return CameraManager; }
    const CWorld& GetWorld()const { return GetCameraManager().GetWorld(); }
#endif
#if defined(FABLE_RETAIL_NCAMERAMODES_CTOOLS_WORLD) || defined(FABLE_RETAIL_NCAMERAMODES_CTOOLS_ROLLING_AVERAGE)
    // Retail opt-in (Codex pass 17): both original PDBs specify this const reference.
    const CGameCameraManager& CameraManager;
#else
    CGameCameraManager* CameraManager;                                // +0x00
#endif
#ifdef FABLE_RETAIL_NCAMERAMODES_CTOOLS_SCAN_THINGS_PASS30
    // Ego_r / FableWin CTools type and 00877C40 both place ScanThings at +4.
    std::vector<CIntelligentPointer<const CThing> > ScanThings;
#else
    unsigned char ScanThings[0xC];                                    // +0x04 vector<CIntelligentPointer<const CThing> >
#endif
    float CameraMeshOffsetDistance;                                   // +0x10
    float CameraCreatureOffsetDistance;                               // +0x14
    float CameraLandscapeOffsetDistance;                              // +0x18
    float CameraThemeOffsetDistance;                                  // +0x1C
};
} // namespace NCameraModes
FABLE_STATIC_ASSERT_AT(114, sizeof(NCameraModes::CTools) == 0x20);
#if defined(FABLE_RETAIL_NCAMERAMODES_CTOOLS_ROLLING_AVERAGE_PAREN) && defined(FABLE_RETAIL_NCAMERAMODES_CTOOLS_ROLLING_OUTOFLINE)
// FITTED (chosen by grading; FableWin /Od size not reproduced). Opt-in (egod e04, row 0x00871FD0 NCameraModes::CStringAndCageBase::UpdateXYVars): the body of the declared-only template
// ??$GetRollingAverage@M@CTools@NCameraModes@@QBEMABM0M@Z (Ego_d / FableWin: no named locals) as an inline out-of-class definition,
// the same arithmetic as the ROLLING_AVERAGE in-class body with the return expression parenthesised: the outer PAREN tuple lets c2
// schedule the caller's `fld [esi+0ACh]` (0x00871FF5) ahead of the first result's dword copy `mov eax,[esp+4]` (0x00871FFB), as retail.
// The other users keep their bytes with this text (0x00697CC0 inline, 0x0084EAF0 / 0x008FCC60 in-row bodies regraded). Needs
// FABLE_RETAIL_NCAMERAMODES_CTOOLS_ROLLING_AVERAGE + _ROLLING_OUTOFLINE. No layout change.
namespace NCameraModes {
template<class T> inline T CTools::GetRollingAverage(const T& current_val,const T& new_val,float weight) const
{return ((current_val*weight+new_val)/(weight+1.0f));}
}
#endif

#ifdef FABLE_RETAIL_NCAMERAMODES_CTOOLS_VECTOR_LOS_PASS30
#include "engine/CThingPhysical.h"
#include "engine/CTCCameraCollision.h"
#include "engine/CTCHitLocations.h"
#include "engine/CPhysicsMeshObject.h"
#include "engine/CMeshIntersectionData.h"
#include "engine/C3DVector.h"

namespace NCameraModes {
inline bool CTools::GetThingPointIntersectionFromTo(const std::vector<const CThing*>& things, const C3DVector& from, const C3DVector& to,
    C3DVector& hitPos, C3DVector& hitNormal, const CThing** hitThing, bool useCameraMeshes, ELOSTest test) const
{
    const CThing* closestThing = 0;
    float closestDistanceSquared = (to - from).GetSquaredMagnitude();
    C3DVector direction = to - from;
    direction.AccurateNormalise();
    C3DVector closestPos;
    C3DVector closestNormal;
    for (std::vector<const CThing*>::const_iterator it = things.begin(); it != things.end(); ++it) {
        const CThing* thing = *it;
        if (thing && !thing->IsDead()) {
            unsigned long flags = thing->GetTypeFlags();
            if ((!(flags & 8) || (test & 8)) && (flags & 2)) {
                C3DVector pos;
                C3DVector normal;
                bool hit = false;
                if (useCameraMeshes) {
                    const CTCCameraCollision* cameraCollision;
                    if (!thing->PeekTC(&cameraCollision) || !cameraCollision->HasCameraPhysicsMeshObject())
                        cameraCollision = 0;
                    if (cameraCollision) {
                        CPhysicsMeshObject mesh = cameraCollision->GetCameraPhysicsMeshObject();
                        CMeshIntersectionData data;
                        if (mesh.GetPointIntersectionFromTo(from, to, data)) {
                            hit = true;
                            pos = data.IntersectionPosition;
                            normal = data.Normal * -1.0f;

                        }
                    }
                    if (!hit) {
                        const CTCHitLocations* hitLocations;
                        if (thing->PeekTC(&hitLocations)) {
                            C3DVector locationPos;
                            C3DVector locationNormal;
                            if (hitLocations->GetPCombatFirstHitLocationIntersectionWithFiniteLine(from, to, locationPos, locationNormal, 0.25f)) {
                                if (cameraCollision || static_cast<const CThingPhysical*>(thing)->IsThingAbleToComeBetweenCameraAndObservedThing()) {
                                    C3DVector centre;
                                    float radius;
                                    hitLocations->GetBoundingSphere(centre, radius, true);
                                    C3DVector testPos;
                                    C3DVector testNormal;
                                    C3DVector start = direction * radius * 2.5f + to;
                                    if (hitLocations->GetPCombatFirstHitLocationIntersectionWithFiniteLine(start, to - direction * 0.2f, testPos, testNormal, 0.25f)) {
                                        pos = locationPos;
                                        normal = locationNormal;
                                        hit = true;
                                    }
                                } else {
                                    pos = locationPos;
                                    normal = locationNormal;
                                    hit = true;
                                }
                            }
                        }
                    }
                } else {
                    const CTCHitLocations* hitLocations;
                    if (thing->PeekTC(&hitLocations) &&
                        hitLocations->GetPCombatFirstHitLocationIntersectionWithFiniteLine(from, to, pos, normal, 0.25f))
                        hit = true;
                }
                if (hit) {
                    float distanceSquared = (pos - from).GetSquaredMagnitude();
                    if (distanceSquared < closestDistanceSquared) {
                        closestDistanceSquared = distanceSquared;
                        closestPos = pos;
                        closestNormal = normal;
                        closestThing = thing;
                    }
                }
            }
        }
    }
    if (closestThing) {
        *hitThing = closestThing;
        hitPos = closestPos;
        hitNormal = closestNormal;
        return true;
    }
    return false;
}
}
// The original 00877840 in cm_helper_functions.obj remains visible at its 00877C40 caller.
extern const float DELTA_NOTIONAL_ZERO;
namespace NCameraModes {
inline bool CTools::IsLineOfSight(const std::vector<const CThing*>& thing_list, const C3DVector& from, const C3DVector& to, C3DVector& collision_pos,
                           C3DVector& normal_vector, float& cam_radius, const CThing** pphit_thing, ELOSTest test_flags, bool camera_test) const
{
    if (!((from - to).GetSquaredMagnitude() > DELTA_NOTIONAL_ZERO * DELTA_NOTIONAL_ZERO))
        return true;
    bool intersection_occured = false;
    float best_cam_rad = 0.0f;
    const CThing* pbest_thing = 0;
    C3DVector best_pos;
    C3DVector best_normal;
    {
        C3DVector test_pos, test_normal;
        const CThing* ptest_thing;
        if ((test_flags & 9) && GetThingPointIntersectionFromTo(thing_list, from, to, test_pos, test_normal, &ptest_thing, camera_test, test_flags)) {
            float test_cam_rad = CameraMeshOffsetDistance;
            if ((ptest_thing->GetTypeFlags() & 8) && (test_flags & 8))
                test_cam_rad = CameraCreatureOffsetDistance;
            best_cam_rad = test_cam_rad;
            best_pos = test_pos;
            best_normal = test_normal;
            pbest_thing = ptest_thing;
            intersection_occured = true;
        }
    }
    if (test_flags & 4) {
        C3DVector test_pos, test_normal;
        if (GetThemePointIntersectionFromTo(from, to, test_pos, test_normal, true)) {
            float test_cam_rad = CameraThemeOffsetDistance;
            if (!intersection_occured || (test_pos - from).GetSquaredMagnitude() < (best_pos - from).GetSquaredMagnitude()) {
                best_pos = test_pos;
                best_normal = test_normal;
                best_cam_rad = test_cam_rad;
            }
            intersection_occured = true;
        }
    }
    if (test_flags & 0x10) {
        C3DVector test_pos, test_normal;
        if (GetThemePointIntersectionFromTo(from, to, test_pos, test_normal, false)) {
            float test_cam_rad = CameraThemeOffsetDistance;
            if (!intersection_occured || (test_pos - from).GetSquaredMagnitude() < (best_pos - from).GetSquaredMagnitude()) {
                best_pos = test_pos;
                best_normal = test_normal;
                best_cam_rad = test_cam_rad;
            }
            intersection_occured = true;
        }
    }
    if (test_flags & 2) {
        C3DVector test_pos, test_normal;
        if (GetLandscapePointIntersectionFromTo(from, to, test_pos, test_normal, camera_test)) {
            float test_cam_rad = CameraLandscapeOffsetDistance;
            if (!intersection_occured || (test_pos - from).GetSquaredMagnitude() < (best_pos - from).GetSquaredMagnitude()) {
                best_pos = test_pos;
                best_normal = test_normal;
                best_cam_rad = test_cam_rad;
            }
            intersection_occured = true;
        }
    }
    if (intersection_occured) {
        collision_pos = best_pos;
        normal_vector = best_normal;
        cam_radius = best_cam_rad;
        *pphit_thing = pbest_thing;
        return false;
    }
    return true;
}
}
#endif
#endif
