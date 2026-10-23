// Grade: DIFFER(3v1499) -- parked x87-heavy row over 1 KB (skeleton only): x87-heavy (353 x87 ops): inline camera matrix/vector interpolation with only two calls (0xB2FD60/0xB2FC50); register-cached x87 stack.
// CEngineCamera::SetCameraState() -- PARK SKELETON (decoded only)
// Leaf: SetCameraState
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B30B50, length 1499 bytes.
// Decode: x87-heavy (353 x87 ops): inline camera matrix/vector interpolation with only two calls (0xB2FD60/0xB2FC50); register-cached x87 stack.
struct CEngineCamera {
    unsigned char _pad[0x100];
    void SetCameraState(const void* state, long a, long b);
};

void CEngineCamera::SetCameraState(const void* state, long a, long b)
{
}
