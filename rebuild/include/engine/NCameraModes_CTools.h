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
#ifdef FABLE_RETAIL_NCAMERAMODES_CTOOLS_MESHTYPE
    const EMeshType TestMeshForOccupationAndGetType(const CPhysicsMeshObject& mesh,const C3DVector& position,const C3DVector& up,float& height) const;   // 0x00875B10
#endif
#ifdef FABLE_RETAIL_NCAMERAMODES_CTOOLS_LANDSCAPEFROMTO
    // Ego_r.pdb ?GetLandscapePointIntersectionFromTo@CTools@NCameraModes@@QBE_NABVC3DVector@@0AAV3@1_N@Z (const). Declaration only.
    bool GetLandscapePointIntersectionFromTo(const C3DVector& from,const C3DVector& to,C3DVector& hitPos,C3DVector& hitNormal,bool includeWater) const;   // 0x00875550
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
#ifdef FABLE_RETAIL_CTCTARGETINGPLAYER_UPDATE_LIST
    enum ELOSTest { TEST_THINGS=1,TEST_LANDSCAPE=2,TEST_THEMES=4,TEST_CREATURES=8,TEST_PASSABILITY=16,TEST_ALL_BUT_CREATURES=7,TEST_ALL=15 };
    bool IsLineOfSight(const C3DVector&,const C3DVector&,C3DVector&,C3DVector&,float&,const CThing**,ELOSTest,bool)const;
#else
    enum ELOSTest { };
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
    unsigned char ScanThings[0xC];                                    // +0x04 vector<CIntelligentPointer<const CThing> >
    float CameraMeshOffsetDistance;                                   // +0x10
    float CameraCreatureOffsetDistance;                               // +0x14
    float CameraLandscapeOffsetDistance;                              // +0x18
    float CameraThemeOffsetDistance;                                  // +0x1C
};
} // namespace NCameraModes
FABLE_STATIC_ASSERT(sizeof(NCameraModes::CTools) == 0x20);
#endif
