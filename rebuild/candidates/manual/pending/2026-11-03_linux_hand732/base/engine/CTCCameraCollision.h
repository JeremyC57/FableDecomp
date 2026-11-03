#ifndef FABLE_ENGINE_CTCCAMERACOLLISION_H
#define FABLE_ENGINE_CTCCAMERACOLLISION_H
// New header (chunk 724 round 3, row 0x008771A0). CTCCameraCollision : CTCBase, Ego_r TPI size 0x10: CameraCollisionDef
// (CDefPointer<const CCameraCollisionDef>) +0x0C. Interface type TCI_CAMERA_COLLISION = 248 (CThing::PeekTC tests
// UsingComponents bit 248 = `test dword [thing+3Ch],1000000h` and keys Components.Find with 0xF8 at 0x00877275..0x008772A2).
// Methods: Ego_r.pdb / FableWin.pdb spellings; retail addresses from the catalog (both called out of line by 0x008771A0).
#include "engine/CTCBase.h"
#include "engine/ETCInterfaceType.h"
class CPhysicsMeshObject;
class CTCCameraCollision : public CTCBase {
public:
    static ETCInterfaceType __fastcall StatGetInterfaceType() { return TCI_CAMERA_COLLISION; }
    bool HasCameraPhysicsMeshObject() const;                          // 0x007E3010 ?HasCameraPhysicsMeshObject@CTCCameraCollision@@QBE_NXZ
    CPhysicsMeshObject GetCameraPhysicsMeshObject() const;            // 0x007E30A0 ?GetCameraPhysicsMeshObject@CTCCameraCollision@@QBE?AVCPhysicsMeshObject@@XZ
private:
    unsigned char CameraCollisionDef[4];                              // +0x0C CDefPointer<const CCameraCollisionDef>
};
FABLE_STATIC_ASSERT(sizeof(CTCCameraCollision) == 0x10);
#endif
