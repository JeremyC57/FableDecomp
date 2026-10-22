#pragma optimize("s", on)
extern void __fastcall CTCUndeadSoul_Destruct(void* soul);
extern void __cdecl OperatorDelete(void* object);
void* __fastcall CTCUndeadSoul_ScalarDeletingDestructor(void* soul, int, unsigned int flags)
{
    CTCUndeadSoul_Destruct(soul);
    if (flags & 1)
        OperatorDelete(soul);
    return soul;
}