#ifndef FABLE_ENGINE_CTCCAMERAPOINTSCRIPTEDSPLINE_H
#define FABLE_ENGINE_CTCCAMERAPOINTSCRIPTEDSPLINE_H
// CTCCameraPointScriptedSpline (agentC37, row 0x0075D580): the scripted camera spline component (RTTI base CTCCameraPointDefinitionBase).
// Members and order: Ego_r.pdb (struct_layouts_egor.tsv, 220 bytes there). Retail offsets differ because the retail STLport vector is 12
// bytes (PDB 16), its iterator a plain pointer and the STLport list 4 bytes (PDB 12): KeyCameras +0x68, PPositionSpline +0x78,
// MyMessages +0x98, MessageToBeSent +0x9C, NextCrossInvalid +0xA0, PauseFor +0xA8, NodeToStartOn +0xB4, NodeToEndOn +0xB8,
// AnimationSpeed +0xBC (rows 0x0075D050, 0x0075CFE0, 0x0075D580), ValidAnimList +0xC0 (row 0x0075D8B0; its retail size is not proven,
// so it is not modelled and the class has no sizeof assert).
// Needs FABLE_RETAIL_CTCBASE, FABLE_RETAIL_CCHARSTRING; the transform helpers need FABLE_RETAIL_CTCCAMERAPOINTDEFINITIONBASE_TRANSFORM.
#include <stddef.h>
#include "rebuild_abi.h"
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/C3DVector.h"
#include "engine/CCountedPointer.h"
#include "engine/CCardinalSpline.h"
#include "engine/CTCCameraPointGeneralCase.h"
class CRightHandedSet;
class CTCCameraPointScriptedSpline : public CTCCameraPointDefinitionBase {
public:
    class CKeyCamera {                                       // 56 bytes (Ego_r.pdb)
    public:
        float GetPauseTime() const { return PauseTime; }     // FableWin ?GetPauseTime@CKeyCamera@CTCCameraPointScriptedSpline@@QBEMXZ
        CCharString GetEvent() const;                        // 0x0075C9B0 FableWin ?GetEvent@CKeyCamera@CTCCameraPointScriptedSpline@@QBE?AVCCharString@@XZ
        float DeltaIndex;                                    // +0x00
        C3DVector Pos;                                       // +0x04
        C3DVector LookDirection;                             // +0x10
        float FOV;                                           // +0x1c
        float Roll;                                          // +0x20
        float ShuttleSpeed;                                  // +0x24
        float Duration;                                      // +0x28
        float PauseTime;                                     // +0x2c
        CCharString EventName;                               // +0x30
        float SpeedFraction;                                 // +0x34
    };
    // Ego_r 0x0076D6E0 / FableWin 0x025C98E0 ?UpdateCameraParams@CTCCameraPointScriptedSpline@@QAEMMAAVC3DVector@@0AAM1AA_NPBV2@PBVCRightHandedSet@@@Z
    float UpdateCameraParams(float index, C3DVector& pos, C3DVector& view_dir, float& fov, float& roll, bool& finished,
                             const C3DVector* origin, const CRightHandedSet* axes);                       // 0x0075D580
    long GetKeyCamera(float index) const;                    // 0x0075CFE0 ?GetKeyCamera@CTCCameraPointScriptedSpline@@QBEJM@Z
    void InitialiseSpline(long start_node, long end_node);   // 0x0075D050 ?InitialiseSpline@CTCCameraPointScriptedSpline@@QAEXJJ@Z

    C3DVector EditPreviewPos;                                // +0x40
    C3DVector EditPreviewViewDirection;                      // +0x4c
    float EditPreviewFOV;                                    // +0x58
    float EditPreviewRoll;                                   // +0x5c
    CCountedPointer<CCardinalSpline> PTimingSpline;          // +0x60
    std::vector<CKeyCamera> KeyCameras;                      // +0x68
    CKeyCamera* SelectedKeyCamera;                           // +0x74 (vector iterator)
    CCountedPointer<CCardinalSpline> PPositionSpline;        // +0x78
    CCountedPointer<CCardinalSpline> PViewDirectionSpline;   // +0x80
    CCountedPointer<CCardinalSpline> PScalarSpline;          // +0x88
    CCountedPointer<CCardinalSpline> PAnimationSpeedSpline;  // +0x90
    std::list<CCharString> MyMessages;                       // +0x98
    CCharString MessageToBeSent;                             // +0x9c
    bool NextCrossInvalid;                                   // +0xa0
    float Tension;                                           // +0xa4
    float PauseFor;                                          // +0xa8
    float TimeToPlay;                                        // +0xac
    float EditPreviewIndex;                                  // +0xb0
    long NodeToStartOn;                                      // +0xb4
    long NodeToEndOn;                                        // +0xb8
    float AnimationSpeed;                                    // +0xbc
};
FABLE_STATIC_ASSERT(offsetof(CTCCameraPointScriptedSpline, PTimingSpline) == 0x60);
FABLE_STATIC_ASSERT(offsetof(CTCCameraPointScriptedSpline, PPositionSpline) == 0x78);
FABLE_STATIC_ASSERT(offsetof(CTCCameraPointScriptedSpline, MessageToBeSent) == 0x9c);
FABLE_STATIC_ASSERT(offsetof(CTCCameraPointScriptedSpline, AnimationSpeed) == 0xbc);
FABLE_STATIC_ASSERT(sizeof(CTCCameraPointScriptedSpline::CKeyCamera) == 0x38);
#endif // FABLE_ENGINE_CTCCAMERAPOINTSCRIPTEDSPLINE_H
