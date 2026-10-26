#pragma optimize("s", on)
extern void __fastcall CTCCarrying_Destruct(void* carrying);
void* __fastcall CTCCarrying_ScalarDeletingDestructor(void* carrying, int, unsigned int flags)
{
    CTCCarrying_Destruct(carrying);
    if (flags & 1)
        operator delete(carrying);
    return carrying;
}