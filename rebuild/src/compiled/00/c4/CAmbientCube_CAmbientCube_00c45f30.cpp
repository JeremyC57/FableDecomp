// CAmbientCube::CAmbientCube(float size) -- build an ambient-sound cube of edge `size`: zero the eight apexes, set up the ramped level (angle flag clear, value 0), clear the apexes' minimum distances, set each apex offset to its unit-cube corner scaled by `size` (corners in X, Y, Z bit order), then centre the cube on the origin
// Leaf: ??0CAmbientCube@@QAE@M@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C45F30, 682 bytes, thiscall `ret 4` (returns `this`).
// Identity (descriptive; libpk ambient sound has no PDB / RTTI): the class of the landed UpdateDistances 0x00C45810 /
//   SetCentre 0x00C458D0 / GetLevel 0x00C45940 / UpdateLevel 0x00C45980 (eight 0x1C-byte apexes, CRampedParam level +0xE0);
//   the one caller 0x00C36524 constructs it at +0x4C of its owner, where the cube forwarders 0x00C361F0 / 0x00C36220 find
//   it. The catalog's "CCamera_InitializeOcclusionGrid" is propagated.
// Callees: CRampedParam::SetImmediate 0x00C0B510, the landed `__fastcall` operator*(const C3DVector&, float) 0x00C258A0
//   (eight times), SetCentre 0x00C458D0.
// Shape: the apexes' own constructor zeroes both vectors (VC7.1 unrolls the eight); the level is set up in the body
//   (`m_level.m_bAngle = false` addressed from `this`, then SetImmediate(0)): an inline CRampedParam constructor addresses the
//   flag from the member pointer (`lea ecx ; mov [ecx+30h]`, 3 bytes off); the free operator is called by name because the
//   shared C3DVector header also declares the member operator*(float).
#include "engine/C3DVector.h"

C3DVector __fastcall operator*(const C3DVector& v, float s);    // 0x00C258A0

class CRampedParam {                                    // 0x34 bytes (the landed 0x00C0B510 model)
public:
    float m_value;                                      // +0x00 current value
    float m_target;                                     // +0x04 target value
    float m_targetRate;                                 // +0x08
    float m_rate;                                       // +0x0c
    float m_10;                                         // +0x10
    float m_time;                                       // +0x14
    float m_duration;                                   // +0x18
    float m_coef[5];                                    // +0x1c
    bool  m_bAngle;                                     // +0x30 wrap the value into [-pi, pi)
    void SetImmediate(float value);                     // 0x00C0B510
};

struct SAmbientApex {                                   // 0x1C bytes
    C3DVector m_offset;                                 // +0x00 corner offset from the cube's centre
    C3DVector m_position;                               // +0x0c corner position
    float     m_minDistance;                            // +0x18 closest listener distance seen
    SAmbientApex() : m_offset(0.0f, 0.0f, 0.0f), m_position(0.0f, 0.0f, 0.0f) {}
};

class CAmbientCube {
public:
    SAmbientApex m_apexes[8];                           // +0x00
    CRampedParam m_level;                               // +0xe0
    CAmbientCube(float size);
    void SetCentre(const C3DVector& centre);            // 0x00C458D0
};

CAmbientCube::CAmbientCube(float size)
{
    m_level.m_bAngle = false;
    m_level.SetImmediate(0.0f);
    for (int i = 0; i < 8; ++i)
        m_apexes[i].m_minDistance = 0.0f;
    m_apexes[0].m_offset = operator*(C3DVector(0.0f, 0.0f, 0.0f), size);
    m_apexes[1].m_offset = operator*(C3DVector(0.0f, 0.0f, 1.0f), size);
    m_apexes[2].m_offset = operator*(C3DVector(0.0f, 1.0f, 0.0f), size);
    m_apexes[3].m_offset = operator*(C3DVector(0.0f, 1.0f, 1.0f), size);
    m_apexes[4].m_offset = operator*(C3DVector(1.0f, 0.0f, 0.0f), size);
    m_apexes[5].m_offset = operator*(C3DVector(1.0f, 0.0f, 1.0f), size);
    m_apexes[6].m_offset = operator*(C3DVector(1.0f, 1.0f, 0.0f), size);
    m_apexes[7].m_offset = operator*(C3DVector(1.0f, 1.0f, 1.0f), size);
    SetCentre(C3DVector(0.0f, 0.0f, 0.0f));
}