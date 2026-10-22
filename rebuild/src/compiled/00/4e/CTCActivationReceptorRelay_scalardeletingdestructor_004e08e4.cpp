#pragma optimize("s", on)
extern void __fastcall CTCActivationReceptorRelay_Destruct(void* relay);
extern void __cdecl OperatorDelete(void* object);
void* __fastcall CTCActivationReceptorRelay_ScalarDeletingDestructor(void* relay, int, unsigned int flags)
{
    CTCActivationReceptorRelay_Destruct(relay);
    if (flags & 1)
        OperatorDelete(relay);
    return relay;
}