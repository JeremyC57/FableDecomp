// CTCLook::SetBodyMoving() -- when the body has to turn, steer the body XY angle (slow in, fast out) towards the look target, pulled back by a quarter of the head turn range unless the look flags turn the body at once; otherwise mark the body turn state done (bit 3) and stop the body
// Grade: DIFFER(296v296) -- dl=15: frame only: retail homes the Sign int in turnAngle's dead word and the body target in targetAngle's word (8-byte frame); ours keeps both locals and packs the temporaries
// Leaf: ?SetBodyMoving@CTCLook@@AAEXXZ
// Flags: none
#include <math.h>
#pragma intrinsic(fmod)

class CThingCreatureBase { public:
    unsigned char _pad00[0x91];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
    bool IsInMelee() const;                     // 0x00661E80 ?IsInMelee@CThingCreatureBase@@QBE_NXZ
};
template<class T> inline bool IS_THING_ALIVE(T* const& p)
{
    if (p != 0) {
        if (!(p->m_flags91 & 1))
            return true;
    }
    return false;
}
class CLookDef { public:
    unsigned char _pad00[0x30];
    float HeadTurnRangeXY;                      // +0x30 (degrees)
};
template<class T> class CDefPointer { public:
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

class CTCLook { public:
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
private:
    float GetHeadTurnRangeXY() const { return 0.5f * (PDef->HeadTurnRangeXY * 0.0027777778f); }       // ?GetHeadTurnRangeXY@CTCLook@@ABEMXZ (inline)
    bool GetTurningAnglesAndCheckForTurningBody(float& targetAngle, float& turnAngle);   // 0x007525C0
    void GetBodySettingsSlowInFastOut(float& angle, float& velocity, float acceleration, float maxSpeed, float targetAngle) const;   // 0x00751970
    void ImplementBodySettings();               // 0x00752300
    void SetBodyMoving();
};

void CTCLook::SetBodyMoving()
{
    if (IS_THING_ALIVE(PParentCreature))
        PParentCreature->IsInMelee();
    float targetAngle;
    float turnAngle;
    if (GetTurningAnglesAndCheckForTurningBody(targetAngle, turnAngle)) {
        long flags = LookFlags;
        float bodyTarget;
        if (!((flags & 1) && !(flags & 6)) && !(flags & 0x80))
            bodyTarget = WrapAngle(targetAngle - GetHeadTurnRangeXY() * Sign(turnAngle) * 0.5f);
        else
            bodyTarget = targetAngle;
        GetBodySettingsSlowInFastOut(BodyAngleXY, BodyVelocityXY, g_BodyTurnAcceleration, g_BodyTurnMaxSpeed, bodyTarget);
        ImplementBodySettings();
        return;
    }
    long state = BodyTurnState | 8;
    if (BodyTurnState != state)
        BodyTurnState = state;
    BodyVelocityXY = 0.0f;
    BodyVelocityYZ = 0.0f;
}
