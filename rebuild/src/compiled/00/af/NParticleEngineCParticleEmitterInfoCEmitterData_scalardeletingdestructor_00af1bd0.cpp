#pragma optimize("t", on)
extern void __fastcall DestructFirstMember(void* member);
extern void __fastcall DestructSecondMember(void* member);
extern void __fastcall CEmitterData_Destruct(void* emitter);
extern void __cdecl OperatorDelete(void* object);
void* __fastcall CEmitterData_CompositeScalarDeletingDestructor(void* emitter, int, unsigned int flags)
{
    DestructFirstMember(static_cast<unsigned char*>(emitter) + 36);
    DestructSecondMember(static_cast<unsigned char*>(emitter) + 28);
    CEmitterData_Destruct(emitter);
    if (flags & 1)
        OperatorDelete(emitter);
    return emitter;
}