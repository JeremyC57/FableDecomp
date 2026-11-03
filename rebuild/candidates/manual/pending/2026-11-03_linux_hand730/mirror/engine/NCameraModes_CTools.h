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
namespace NCameraModes {
class CTools {
public:
    enum ELOSTest { };
    bool GetThingPointIntersectionFromTo(const std::vector<const CThing*>&, const C3DVector&, const C3DVector&, C3DVector&,
        C3DVector&, const CThing**, bool, ELOSTest) const;           // 0x008771A0
    // chunk 724c: the thing-vector IsLineOfSight overload, Ego_r ?IsLineOfSight@CTools@NCameraModes@@QBE_NABV?$vector@PBVCThing@@...
    // ABVC3DVector@@1AAV5@2AAMPAPBVCThing@@W4ELOSTest@12@_N@Z = retail 0x00877840 (the landed 0x00877C40 calls it).
    bool IsLineOfSight(const std::vector<const CThing*>&, const C3DVector&, const C3DVector&, C3DVector&, C3DVector&, float&,
        const CThing**, ELOSTest, bool) const;                           // 0x00877840
private:
    CGameCameraManager* CameraManager;                                // +0x00
    unsigned char ScanThings[0xC];                                    // +0x04 vector<CIntelligentPointer<const CThing> >
    float CameraMeshOffsetDistance;                                   // +0x10
    float CameraCreatureOffsetDistance;                               // +0x14
    float CameraLandscapeOffsetDistance;                              // +0x18
    float CameraThemeOffsetDistance;                                  // +0x1C
};
} // namespace NCameraModes
FABLE_STATIC_ASSERT(sizeof(NCameraModes::CTools) == 0x20);
#endif
