#pragma optimize("s", on)
extern void __fastcall CTCMoneyReceiver_Destruct(void* receiver);
extern void __cdecl OperatorDelete(void* object);
void* __fastcall CTCMoneyReceiver_ScalarDeletingDestructor(void* receiver, int, unsigned int flags)
{
    CTCMoneyReceiver_Destruct(receiver);
    if (flags & 1)
        OperatorDelete(receiver);
    return receiver;
}