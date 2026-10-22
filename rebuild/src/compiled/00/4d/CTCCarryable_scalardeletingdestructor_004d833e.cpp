#pragma optimize("s", on)
extern void __fastcall CTCCarryable_Destruct(void* carryable);
extern void __cdecl OperatorDelete(void* object);
void* __fastcall CTCCarryable_ScalarDeletingDestructor(void* carryable, int, unsigned int flags)
{
    CTCCarryable_Destruct(carryable);
    if (flags & 1)
        OperatorDelete(carryable);
    return carryable;
}