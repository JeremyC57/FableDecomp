#pragma optimize("s", on)
extern void __fastcall CParticleAttacherDef_Destruct(void* def);
extern void __cdecl OperatorDelete(void* object);
void* __fastcall CParticleAttacherDef_ScalarDeletingDestructor(void* def, int, unsigned int flags)
{
    CParticleAttacherDef_Destruct(def);
    if (flags & 1)
        OperatorDelete(def);
    return def;
}