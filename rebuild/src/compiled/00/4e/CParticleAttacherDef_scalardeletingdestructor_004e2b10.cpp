#pragma optimize("s", on)
extern void __fastcall CParticleAttacherDef_Destruct(void* def);
void* __fastcall CParticleAttacherDef_ScalarDeletingDestructor(void* def, int, unsigned int flags)
{
    CParticleAttacherDef_Destruct(def);
    if (flags & 1)
        operator delete(def);
    return def;
}