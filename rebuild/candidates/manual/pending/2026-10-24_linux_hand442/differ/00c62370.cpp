// RandomVectorInSphere(float radius) -- random direction (theta = 2*pi*u, phi = acos(2u-1)) scaled by a random length up to radius
// Grade: DIFFER(126v118) -- LTCG lane gap: retail takes the result pointer in esi (caller 0x00C625A7 `lea esi,[esp+14h]`) and keeps x87 values live across calls to 0x00C62350
// Leaf: ?RandomVectorInSphere@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C62370, 118 bytes (`ret 4`; hidden return pointer in esi, returned in eax).
// Callees: RandomUnit 0x00C62350 (`rand() * (1/32767)`, three calls, st0 = phi held across the third), MSVCR71 `_CIacos`
//   (thunk 0x00BFEBA2); `fsin`/`fcos` are the /Oi intrinsics. Constants 6.2831855f (0x0128F608) and 1.0f (0x0122DED8).
// Identity: the manifest's "Quaternion_FromAxisAngle" is a propagated label: the body builds a three-float vector
//   x = sin(phi)cos(theta)r, y = sin(theta)cos(phi)r, z = cos(phi)r (retail's y really uses cos(phi)). Descriptive name.
extern "C" __declspec(dllimport) int __cdecl rand(void);
extern "C" double __cdecl acos(double x);
extern "C" double __cdecl sin(double x);
extern "C" double __cdecl cos(double x);
#pragma intrinsic(acos, sin, cos)

struct CPKVector3 {
    float x, y, z;
};

#pragma auto_inline(off)
__declspec(noinline) float RandomUnit()           // 0x00C62350
{
    return (float)rand() * (1.0f / 32767.0f);
}
#pragma auto_inline(on)

CPKVector3 RandomVectorInSphere(float radius)
{
    float theta = RandomUnit() * 6.2831855f;
    double phi = acos(RandomUnit() * 2.0f - 1.0f);
    float r = RandomUnit() * radius;
    CPKVector3 v;
    v.x = (float)(sin(phi) * cos(theta) * r);
    v.y = (float)(cos(phi) * sin(theta) * r);
    v.z = (float)(cos(phi) * r);
    return v;
}
