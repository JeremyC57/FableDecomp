#pragma optimize("s", on)
extern void __fastcall CTCCarriedActionUseGoldBag_Destruct(void* bag);
extern void __cdecl OperatorDelete(void* object);
void* __fastcall CTCCarriedActionUseGoldBag_ScalarDeletingDestructor(void* bag, int, unsigned int flags)
{
    CTCCarriedActionUseGoldBag_Destruct(bag);
    if (flags & 1)
        OperatorDelete(bag);
    return bag;
}