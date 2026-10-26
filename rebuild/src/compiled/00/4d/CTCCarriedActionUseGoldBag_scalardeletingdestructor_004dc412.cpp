#pragma optimize("s", on)
extern void __fastcall CTCCarriedActionUseGoldBag_Destruct(void* bag);
void* __fastcall CTCCarriedActionUseGoldBag_ScalarDeletingDestructor(void* bag, int, unsigned int flags)
{
    CTCCarriedActionUseGoldBag_Destruct(bag);
    if (flags & 1)
        operator delete(bag);
    return bag;
}