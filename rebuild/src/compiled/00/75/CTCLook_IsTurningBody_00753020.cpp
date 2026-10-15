// CTCLook::IsTurningBody() -- ask GetTurningAnglesAndCheckForTurningBody whether the body must turn, discarding the two angles it writes back
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00753020, length 21 bytes.
// Callees: CTCLook::GetTurningAnglesAndCheckForTurningBody(float*, float*) @ 0x007525C0 (__fastcall, `ret 8`; `this` stays in ecx).
// Called from CTCLook::LookAtPos (0x00754003), so this is the genuine CTCLook method. Shape: two float out-locals in an
// 8-byte frame; the YZ angle is the first local ([esp]) and its address is pushed first (second argument), the XY angle
// sits at [esp+4]; `add esp,8` releases the frame after the callee popped its arguments, the bool result passes through al.
struct CTCLook {
    bool GetTurningAnglesAndCheckForTurningBody(float* angleXY, float* angleYZ);   // 0x007525C0
    bool IsTurningBody();
};

bool CTCLook::IsTurningBody()
{
    float angleXY;
    float angleYZ;
    return GetTurningAnglesAndCheckForTurningBody(&angleXY, &angleYZ);
}