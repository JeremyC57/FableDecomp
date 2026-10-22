#pragma optimize("s", on)
extern void __fastcall CTCBlacksmiths_Destruct(void* blacksmiths);
extern void __cdecl OperatorDelete(void* object);
void* __fastcall CTCBlacksmiths_ScalarDeletingDestructor(void* blacksmiths, int, unsigned int flags)
{
    CTCBlacksmiths_Destruct(blacksmiths);
    if (flags & 1)
        OperatorDelete(blacksmiths);
    return blacksmiths;
}