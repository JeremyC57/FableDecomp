#pragma optimize("s", on)
extern void __fastcall CTCActivationReceptorCreatureGenerator_Destruct(void* generator);
extern void __cdecl OperatorDelete(void* object);
void* __fastcall CTCActivationReceptorCreatureGenerator_ScalarDeletingDestructor(void* generator, int, unsigned int flags)
{
    CTCActivationReceptorCreatureGenerator_Destruct(generator);
    if (flags & 1)
        OperatorDelete(generator);
    return generator;
}