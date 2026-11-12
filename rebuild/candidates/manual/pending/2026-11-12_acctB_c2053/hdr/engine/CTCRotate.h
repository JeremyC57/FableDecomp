#ifndef FABLE_ENGINE_CTCROTATE_H
#define FABLE_ENGINE_CTCROTATE_H
// New shared header (chunk 2053, row 0x0087F3A0): CTCRotate, Ego_r.pdb layout (56 bytes: CTCBase 0xC, RotationAxis +0x0C,
// RotationSpeed +0x18, NumRotations +0x1C, Timer +0x20, TimerLimit +0x24, Rotating +0x28, ObjectToRotateAround +0x2C, Index +0x30,
// DummyRatherThanHelper +0x34). FableWin.pdb ?SetRotating@CTCRotate@@QAEX_N@Z (inline: the row stores `mov byte ptr [edi+28h],1`,
// 0x0087FC11) and ?SetRotationInfo@CTCRotate@@QAEXABVC3DVector@@MM@Z (out of line, retail 0x0072A530, landed). Needs FABLE_RETAIL_CTCBASE,
// fable_string.h and engine/C3DVector.h.
#include "engine/CTCBase.h"
class CTCRotate : public CTCBase {
public:
    void SetRotating(bool rotating) { Rotating = rotating; }
    void SetRotationInfo(const C3DVector& axis, float speed, float num_rotations);   // 0x0072A530
protected:
    C3DVector   RotationAxis;                                            // +0x0c
    float       RotationSpeed;                                           // +0x18
    float       NumRotations;                                            // +0x1c
    long        Timer;                                                   // +0x20
    long        TimerLimit;                                              // +0x24
    bool        Rotating;                                                // +0x28
    CCharString ObjectToRotateAround;                                    // +0x2c
    long        Index;                                                   // +0x30
    bool        DummyRatherThanHelper;                                   // +0x34
};
#endif
