// CTCLook::SetBodyMoving() -- when the body has to turn, steer the body XY angle (slow in, fast out) towards the look target, pulled back by a quarter of the head turn range unless the look flags turn the body at once; otherwise mark the body turn state done (bit 3) and stop the body
// Grade: DIFFER(272v296) -- frame: retail homes the Sign int in the dead turnAngle word and the wrapped target in targetAngle's word (8-byte frame) and copies the target through a separate jump-back arm; VC7.1 here gives the temporaries their own word (0xc frame). Tried: bodyTarget local, ternary, if-assign, declaration order, Sign by reference, operand order.
#include <math.h>
#pragma intrinsic(fmod)

struct CThingCreatureBase {
    unsigned char _pad00[0x91];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
    bool IsIncapacitated();                     // 0x00661E80
};
template<class T> inline bool IS_THING_ALIVE(T* const& p)
{
    if (p != 0) {
        if (!(p->m_flags91 & 1))
            return true;
    }
    return false;
}
struct CLookDef {
    unsigned char _pad00[0x30];
    float HeadTurnRangeXY;                      // +0x30 (degrees)
};
template<class T> struct CDefPointer {
    T* Object;
    T* operator->() const { return Object; }
};
extern float g_BodyTurnAcceleration;           // 0x01382698 (.data tunable)
extern float g_BodyTurnMaxSpeed;               // 0x0138269C (.data tunable)

inline int Sign(float x) { return x < 0.0f ? -1 : 1; }
__forceinline float WrapAngle(float angle)
{
    if (angle < 0.0f || angle >= 1.0f) {
        angle = (float)fmod((double)angle, 1.0);
        if (angle < 0.0f)
            angle += 1.0f;
    }
    return angle;
}

struct CTCLook {
    void*               __vftable;              // +0x00
    void*               ParentThing;            // +0x04
    unsigned char       _pad08[0x10 - 0x08];
    CThingCreatureBase* PParentCreature;        // +0x10
    CDefPointer<const CLookDef> PDef;           // +0x14
    void*               PAppearanceDef;         // +0x18
    float RecoveryAngleToLookAtXY;              // +0x1c
    float RecoveryAngleToLookAtYZ;              // +0x20
    float BodyAngleXY;                          // +0x24
    float BodyAngleYZ;                          // +0x28
    unsigned char       _pad2c[0x4c - 0x2c];
    long  BodyTurnState;                        // +0x4c
    bool  CanTurnBody;                          // +0x50
    float BodyVelocityXY;                       // +0x54
    float BodyVelocityYZ;                       // +0x58
    float HeadVelocityXY;                       // +0x5c
    float HeadVelocityYZ;                       // +0x60
    long  LookFlags;                            // +0x64
    float GetHeadTurnRangeXY() const { return PDef->HeadTurnRangeXY / 360.0f; }
    bool GetTurningAnglesAndCheckForTurningBody(float& targetAngle, float& turnAngle);   // 0x007525C0
    void GetBodySettingsSlowInFastOut(float& angle, float& velocity, float acceleration, float maxSpeed, float targetAngle) const;   // 0x00751970
    void ImplementBodySettings();               // 0x00752300
    void SetBodyMoving();
};

void CTCLook::SetBodyMoving()
{
    if (IS_THING_ALIVE(PParentCreature))
        PParentCreature->IsIncapacitated();
    float targetAngle;
    float turnAngle;
    if (GetTurningAnglesAndCheckForTurningBody(targetAngle, turnAngle)) {
        long flags = LookFlags;
        if (!((flags & 1) && !(flags & 6)) && !(flags & 0x80))
            targetAngle = WrapAngle(targetAngle - Sign(turnAngle) * GetHeadTurnRangeXY() * 0.25f);
        GetBodySettingsSlowInFastOut(BodyAngleXY, BodyVelocityXY, g_BodyTurnAcceleration, g_BodyTurnMaxSpeed, targetAngle);
        ImplementBodySettings();
        return;
    }
    long state = BodyTurnState | 8;
    if (BodyTurnState != state)
        BodyTurnState = state;
    BodyVelocityXY = 0.0f;
    BodyVelocityYZ = 0.0f;
}
