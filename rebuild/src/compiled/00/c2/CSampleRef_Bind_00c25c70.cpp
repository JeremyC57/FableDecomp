// CSampleRef::Bind(CSoundSample* sample, long key1, long key2, long key3) -- bind to a sample (adding a reference) and record three keys
// Leaf: ?Bind@CSampleRef@@QAEXPAVCSoundSample@@JJJ@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C25C70, 37 bytes (+ padding), `ret 10h`.
// Callees: CSoundSample::AddRef() @ 0x00C1EA30 (`inc word ptr [ecx+1Eh]`, called on the ARGUMENT; the spelling the landed binders use).
// Identity (NF42, round 5): libpk/CSampleRef.h declares Bind at 0x00C25C70 (descriptive libpk names); the landed 0x00C11230 binds it.
// Was the address-named CParticleRef_00c25c70::Bind on a local CParticle model; now on the shared header. CSoundSample has no shared
// header: only the called member is declared here.
#include "libpk/CSampleRef.h"
class CSoundSample { public: void AddRef(); };   // 0x00C1EA30

void CSampleRef::Bind(CSoundSample* sample, long key1, long key2, long key3)
{
    Sample = sample;
    sample->AddRef();
    Key1 = key1;
    Key2 = key2;
    Key3 = key3;
}
