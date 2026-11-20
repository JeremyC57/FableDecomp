// CTCCameraPointScripted::GetLookDirection(const CRightHandedSet* axes) const -- the scripted camera's start look direction: LookPos (+0x58)
//   transformed into the given frame
// Leaf: ?GetLookDirection@CTCCameraPointScripted@@QBE?AVC3DVector@@PBVCRightHandedSet@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0075ccb0, 50 bytes, `ret 8` (hidden result + axes).
// Identity (NF120, PRIORITY q4416): Ego_r.pdb 0x0076BB30 / FableWin.pdb 0x025C6FB0 ?GetLookDirection@CTCCameraPointScripted@@QBE?AVC3DVector@@PBVCRightHandedSet@@@Z;
//   the body transforms +0x58 (Ego_r.pdb CTCCameraPointScripted::LookPos) through TransformVector; the landed CameraUseCameraPoint
//   0x0089E190 calls it with (hidden result, rhSet) for the start look-at and binds this spelling (RELOCATION_MATCH). Was the stand-in
//   CEnvironmentSpellEffectInterface::GetRenderFrameInterpolatedScreenFilterParameters over a 12-byte SFilterParams record.
// Callee: the protected const ?TransformVector@CTCCameraPointDefinitionBase@@IBE?AVC3DVector@@ABV2@PBVCRightHandedSet@@@Z 0x0075C280
//   (Ego_r.pdb / FableWin.pdb; the spelling the landed TransformPosition 0x0075C420 binds).
// Layout: Ego_r.pdb CTCCameraPointScripted (CTCCameraPointDefinitionBase 0x40, StartPos +0x40, EndPos +0x4C, LookPos +0x58,
//   LookPosEnd +0x64). No engine header exists for either class: local models (the row's model, respelled in place).
// Shape: the result is built in a 12-byte local and copied out (X through the x87, Y / Z as integers): a named local returned by value.
#include "engine/C3DVector.h"
class CRightHandedSet;

class CTCCameraPointDefinitionBase { public:
    unsigned char _pad[0x40];                           // CTCBase .. Activated (+0x3C), padded to 0x40
protected:
    C3DVector TransformVector(const C3DVector& vector, const CRightHandedSet* axes) const;   // 0x0075C280
};

class CTCCameraPointScripted : public CTCCameraPointDefinitionBase { public:
    C3DVector StartPos;                                 // +0x40
    C3DVector EndPos;                                   // +0x4c
    C3DVector LookPos;                                  // +0x58
    C3DVector LookPosEnd;                               // +0x64
    C3DVector GetLookDirection(const CRightHandedSet* axes) const;
};

C3DVector CTCCameraPointScripted::GetLookDirection(const CRightHandedSet* axes) const
{
    C3DVector direction = TransformVector(LookPos, axes);
    return direction;
}
