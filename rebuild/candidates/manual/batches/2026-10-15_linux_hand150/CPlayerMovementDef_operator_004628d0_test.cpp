// behaviour test for 0x004628D0 CPlayerMovementDef::operator=
#include <cstdio>
struct CParentDefClassBase {
    int baseTag;
    CParentDefClassBase& operator=(const CParentDefClassBase& o) { baseTag = o.baseTag; return *this; }
};
struct CPlayerMovementDef : CParentDefClassBase {
    float JoystickMovementStationaryBand, JoystickMovementSlowBand, JoystickMovementWalkingBand, JoystickMovementJoggingBand;
    float MinRequiredAngleFor180, SecondsToStoreLastMovementFor;
    long NoMovementFramesToCheckFor180, NoMovementFramesToCheckFor180PriorStraightLine;
    float MinNoRunSecondsForSkidHaltMove; long NoRunningFramesToPreventSkidHalt;
    float MaxAllowedAngleChangeForSkidHalt, MaxTurnSpeed, MaxTurnSpeedHeavyWeapon, LeanAngleTurnFactor, LeanAngleLimitAngle, LeanAngleStepPerFrame;
    CPlayerMovementDef& operator=(const CPlayerMovementDef& other)
    {
        CParentDefClassBase::operator=(other);
        JoystickMovementStationaryBand = other.JoystickMovementStationaryBand;
        JoystickMovementSlowBand = other.JoystickMovementSlowBand;
        JoystickMovementWalkingBand = other.JoystickMovementWalkingBand;
        JoystickMovementJoggingBand = other.JoystickMovementJoggingBand;
        MinRequiredAngleFor180 = other.MinRequiredAngleFor180;
        SecondsToStoreLastMovementFor = other.SecondsToStoreLastMovementFor;
        NoMovementFramesToCheckFor180 = other.NoMovementFramesToCheckFor180;
        NoMovementFramesToCheckFor180PriorStraightLine = other.NoMovementFramesToCheckFor180PriorStraightLine;
        MinNoRunSecondsForSkidHaltMove = other.MinNoRunSecondsForSkidHaltMove;
        NoRunningFramesToPreventSkidHalt = other.NoRunningFramesToPreventSkidHalt;
        MaxAllowedAngleChangeForSkidHalt = other.MaxAllowedAngleChangeForSkidHalt;
        MaxTurnSpeed = other.MaxTurnSpeed;
        MaxTurnSpeedHeavyWeapon = other.MaxTurnSpeedHeavyWeapon;
        LeanAngleTurnFactor = other.LeanAngleTurnFactor;
        LeanAngleLimitAngle = other.LeanAngleLimitAngle;
        LeanAngleStepPerFrame = other.LeanAngleStepPerFrame;
        return *this;
    }
};
int main()
{
    CPlayerMovementDef a, b;
    b.baseTag = 7; b.JoystickMovementStationaryBand = 0.1f; b.NoMovementFramesToCheckFor180 = 12;
    b.NoRunningFramesToPreventSkidHalt = 5; b.LeanAngleStepPerFrame = 3.5f; b.MaxTurnSpeed = 2.0f;
    a.baseTag = 0; a.JoystickMovementStationaryBand = 0; a.NoMovementFramesToCheckFor180 = 0;
    a.NoRunningFramesToPreventSkidHalt = 0; a.LeanAngleStepPerFrame = 0; a.MaxTurnSpeed = 0;
    CPlayerMovementDef* r = &(a = b);
    if (r != &a || a.baseTag != 7 || a.JoystickMovementStationaryBand != 0.1f || a.NoMovementFramesToCheckFor180 != 12
        || a.NoRunningFramesToPreventSkidHalt != 5 || a.LeanAngleStepPerFrame != 3.5f || a.MaxTurnSpeed != 2.0f)
    { std::printf("004628d0_TEST FAIL\n"); return 1; }
    std::printf("004628d0_TEST PASS\n");
    return 0;
}
