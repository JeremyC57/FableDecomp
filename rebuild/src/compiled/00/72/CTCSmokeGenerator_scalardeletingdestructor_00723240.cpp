#pragma optimize("t", on)
extern void __fastcall DestructMember_00722eb0(void* member);
extern void __fastcall CTCSmokeGenerator_Destruct(void* generator);
extern void __cdecl OperatorDelete(void* object);
void* __fastcall CTCSmokeGenerator_CompositeScalarDeletingDestructor(void* generator, int, unsigned int flags)
{
    DestructMember_00722eb0(static_cast<unsigned char*>(generator) + 16);
    CTCSmokeGenerator_Destruct(generator);
    if (flags & 1)
        OperatorDelete(generator);
    return generator;
}