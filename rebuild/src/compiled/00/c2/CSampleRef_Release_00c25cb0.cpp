// CSampleRef::Release() -- clear the three keys, drop the sample's reference and null the pointer
// Leaf: ?Release@CSampleRef@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C25CB0, 39 bytes, `ret` (callers 0x00C11294, 0x00C11A52, 0x00C2169D).
// Callees: CSoundSample::Release() @ 0x00C1EA40 (`dec word ptr [ecx+1Eh]`; vendor naming V2 spelling).
// Identity (NF42, round 5): libpk/CSampleRef.h declares Release at 0x00C25CB0 (descriptive libpk names); the landed 0x00C11230 binds
// ?Release@CSampleRef@@QAEXXZ here. Was `CParticleHandle::~CParticleHandle` on a local model; now on the shared header.
// Shape: the pointer is loaded first and the zero stores are scheduled around it; no null test before the call.
#include "libpk/CSampleRef.h"
class CSoundSample { public: void Release(); };   // 0x00C1EA40

void CSampleRef::Release()
{
    Key1 = 0;
    Key2 = 0;
    Key3 = 0;
    Sample->Release();
    Sample = 0;
}
