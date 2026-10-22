#pragma optimize("s", on)
extern void __fastcall CTCGift_Destruct(void* gift);
extern void __cdecl OperatorDelete(void* object);
void* __fastcall CTCGift_ScalarDeletingDestructor(void* gift, int, unsigned int flags)
{
    CTCGift_Destruct(gift);
    if (flags & 1)
        OperatorDelete(gift);
    return gift;
}