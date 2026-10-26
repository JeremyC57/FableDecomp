#pragma optimize("s", on)
extern void __fastcall CTCActivationReceptorCreatureGenerator_Destruct(void* generator);
void* __fastcall CTCActivationReceptorCreatureGenerator_ScalarDeletingDestructor(void* generator, int, unsigned int flags)
{
    CTCActivationReceptorCreatureGenerator_Destruct(generator);
    if (flags & 1)
        operator delete(generator);
    return generator;
}