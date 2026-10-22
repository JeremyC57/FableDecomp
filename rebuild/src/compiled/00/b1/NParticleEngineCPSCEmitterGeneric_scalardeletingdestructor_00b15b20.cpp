#pragma optimize("t", on)
extern void __fastcall DestructFirstMember(void* member);
extern void __fastcall DestructSecondMember(void* member);
extern void __fastcall CPSCEmitterGeneric_Destruct(void* emitter);
extern void __cdecl OperatorDelete(void* object);
void* __fastcall CPSCEmitterGeneric_CompositeScalarDeletingDestructor(void* emitter, int, unsigned int flags)
{
    DestructFirstMember(static_cast<unsigned char*>(emitter) + 24);
    DestructSecondMember(static_cast<unsigned char*>(emitter) + 12);
    CPSCEmitterGeneric_Destruct(emitter);
    if (flags & 1)
        OperatorDelete(emitter);
    return emitter;
}