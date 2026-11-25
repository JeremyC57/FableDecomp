// Float literals (NF418, 2026-11-25): retail's merged __real@ pool constants were modelled as data: g_zeroTime (table ?g_zeroTime@@3MB -> 0x0122DEDC) -> 0.0f. Spelled at the use
//   site; VC7.1 references a plain float literal through __real@<bits> (COMDAT, exe-wide merged), e.g. `return 0.0f;` -> fld [__real@00000000].

struct CParticleSystem {
    char pad[0xb0];
    float totalTime;   /* +0xb0 */
    float elapsed;     /* +0xb4 */
};

float __fastcall CParticleSystem_GetNormalisedTime(const CParticleSystem* self)
{
    if (self->totalTime > 0.0f)
        return self->elapsed / self->totalTime;
    return 0.0f;
}