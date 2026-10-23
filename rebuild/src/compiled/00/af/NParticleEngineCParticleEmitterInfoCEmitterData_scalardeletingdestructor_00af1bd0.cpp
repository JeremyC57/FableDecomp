#pragma optimize("t", on)
extern void __fastcall DestructFirstMember_00ae94a0(void* member);
extern void __fastcall DestructSecondMember_009a4400(void* member);
extern void __fastcall CEmitterData_Destruct(void* emitter);
extern void __cdecl OperatorDelete(void* object);
void* __fastcall CEmitterData_CompositeScalarDeletingDestructor(void* emitter, int, unsigned int flags)
{
    DestructFirstMember_00ae94a0(static_cast<unsigned char*>(emitter) + 36);
    DestructSecondMember_009a4400(static_cast<unsigned char*>(emitter) + 28);
    CEmitterData_Destruct(emitter);
    if (flags & 1)
        OperatorDelete(emitter);
    return emitter;
}