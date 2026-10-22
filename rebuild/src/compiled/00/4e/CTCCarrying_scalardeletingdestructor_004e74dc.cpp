#pragma optimize("s", on)
extern void __fastcall CTCCarrying_Destruct(void* carrying);
extern void __cdecl OperatorDelete(void* object);
void* __fastcall CTCCarrying_ScalarDeletingDestructor(void* carrying, int, unsigned int flags)
{
    CTCCarrying_Destruct(carrying);
    if (flags & 1)
        OperatorDelete(carrying);
    return carrying;
}