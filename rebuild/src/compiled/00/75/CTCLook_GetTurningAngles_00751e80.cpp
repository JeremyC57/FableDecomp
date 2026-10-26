// CTCLook::GetTurningAngles(float& targetAngle, float& turnAngle) -- when the body may turn (state bit 1, no blocking action, not bit 3), the XY angle to look along (direction or look-at position, else the recovery angle) and its difference from the body facing
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00751E80, length 304 bytes (+ int3 padding), `ret 8`, bool in al. Called by
// GetTurningAnglesAndCheckForTurningBody 0x007525C0 (landed).
// Callees: CThingCreatureBase::IsPerformingAnyAction() @ 0x006622A0, the current action's virtual slot 50 (+0xC8, a
// "blocks body turning" query; name descriptive), the physics TC's GetFacingAngleXY (slot 63, +0xFC), _CIfmod @ 0x00BFEB28,
// AngleBetweenPositions @ 0x00661540 (`__fastcall` (from, to): the position compare's own ecx/edx bases are reused),
// GFGetSignedAngleDifference @ 0x00A441E0 (`__fastcall`; its two floats still go on the stack, `ret 8`). Constants: 1/(2pi) 0x01237F08, 0.0f 0x0122DEDC, 1.0f 0x0122DED8, the double
// 1.0 0x01237F00, 1e-4f 0x0129BA3C.
// Shape: nested ifs with the single `return false` last; IS_THING_ALIVE is the `T* const&` template helper (eax test, ecx
// copy); the direction arm is `WrapAngle(atan2(X, Y) * 1/(2pi))` (fpatan; __forceinline wrap into [0, 1)); the position arm
// is an inline __forceinline C3DVector::operator!= with the 1e-4 tolerance; the recovery angle is stored first and only
// overwritten by the arms.
// Layout (retail CTCLook, PDB offsets below the vector): ParentThing +0x04, PParentCreature +0x10, RecoveryAngleToLookAtXY
// +0x1c, BodyTurnState +0x4c (bit flags 2 / 8 tested), LookFlags +0x64 (bit 6), PosToLookAt +0x68, DirectionToLookIn +0x74;
// CThingCreatureBase dead bit +0x91, current action +0x100; CTCPhysicsBase Position +0x0c. No engine header for CTCLook.
#include <math.h>
#pragma intrinsic(atan2, fmod, fabs)

extern float g_epsilon_0129ba3c;                                                      // 0x0129BA3C (1e-4f)
class C3DVector { public:
    float X, Y, Z;
    __forceinline bool operator!=(const C3DVector& o) const
    {
        return (float)fabs(X - o.X) > g_epsilon_0129ba3c || (float)fabs(Y - o.Y) > g_epsilon_0129ba3c ||
               (float)fabs(Z - o.Z) > g_epsilon_0129ba3c;
    }
};

#define FILL10(n) virtual void v##n##0(); virtual void v##n##1(); virtual void v##n##2(); virtual void v##n##3(); \
    virtual void v##n##4(); virtual void v##n##5(); virtual void v##n##6(); virtual void v##n##7(); \
    virtual void v##n##8(); virtual void v##n##9();
class CTCPhysicsBase { public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    FILL10(1) FILL10(2) FILL10(3) FILL10(4) FILL10(5)
    virtual void v60(); virtual void v61(); virtual void v62();
    virtual float GetFacingAngleXY();           // slot 63 (+0xFC)
    void*     m_tcBase[2];                      // +0x04 CTCBase body
    C3DVector Position;                         // +0x0c
};
class CThing { public:
    unsigned char _pad00[0x60];
    CTCPhysicsBase* PhysicsTC;                  // +0x60
};
class CCreatureActionBase { public:
    FILL10(0) FILL10(1) FILL10(2) FILL10(3) FILL10(4)
    virtual bool BlocksBodyTurning();           // slot 50 (+0xC8)
};
class CThingCreatureBase { public:
    unsigned char _pad00[0x91];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
    unsigned char _pad92[0x100 - 0x92];
    CCreatureActionBase* CurrentAction;         // +0x100 (CCountedPointer<CCreatureActionBase> data)
public:
    bool IsPerformingAnyAction() const;   // 0x006622A0
};
template<class T> inline bool IS_THING_ALIVE(T* const& p)
{
    if (p != 0) {
        if (!(p->m_flags91 & 1))
            return true;
    }
    return false;
}

float __fastcall AngleBetweenPositions(const C3DVector& from, const C3DVector& to);   // 0x00661540
float __fastcall GFGetSignedAngleDifference(float a, float b);                        // 0x00A441E0 (Ego_r.pdb ?GFGetSignedAngleDifference@@YIMMM@Z)
extern float g_epsilon_0129ba3c;                                                      // 0x0129BA3C (1e-4f)

__forceinline float WrapAngle(float angle)
{
    if (angle < 0.0f || angle >= 1.0f) {
        angle = (float)fmod((double)angle, 1.0);
        if (angle < 0.0f)
            angle += 1.0f;
    }
    return angle;
}

class CTCLook { public:
    void*               __vftable;              // +0x00
    CThing*             ParentThing;            // +0x04
    unsigned char       _pad08[0x10 - 0x08];
    CThingCreatureBase* PParentCreature;        // +0x10
    unsigned char       _pad14[0x1c - 0x14];
    float RecoveryAngleToLookAtXY;              // +0x1c
    float RecoveryAngleToLookAtYZ;              // +0x20
    unsigned char       _pad24[0x4c - 0x24];
    long  BodyTurnState;                        // +0x4c
    unsigned char       _pad50[0x64 - 0x50];
    long  LookFlags;                            // +0x64
    C3DVector PosToLookAt;                      // +0x68
    C3DVector DirectionToLookIn;                // +0x74
    bool GetTurningAngles(float& targetAngle, float& turnAngle);
};

bool CTCLook::GetTurningAngles(float& targetAngle, float& turnAngle)
{
    if (BodyTurnState & 2) {
        if (!(IS_THING_ALIVE(PParentCreature) && PParentCreature->IsPerformingAnyAction() &&
              PParentCreature->CurrentAction->BlocksBodyTurning()) && !(BodyTurnState & 8)) {
            CTCPhysicsBase* physics = ParentThing->PhysicsTC;
            float bodyXY = physics->GetFacingAngleXY();
            targetAngle = RecoveryAngleToLookAtXY;
            if (LookFlags & 0x40) {
                targetAngle = WrapAngle((float)atan2(DirectionToLookIn.X, DirectionToLookIn.Y) * 0.15915494f);
            } else if (physics->Position != PosToLookAt) {
                targetAngle = AngleBetweenPositions(physics->Position, PosToLookAt);
            }
            turnAngle = GFGetSignedAngleDifference(bodyXY, targetAngle);
            return true;
        }
    }
    return false;
}