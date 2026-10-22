#pragma optimize("s", on)
extern void __fastcall CTCCreatureGroupTag_Destruct(void* tag);
extern void __cdecl OperatorDelete(void* object);
void* __fastcall CTCCreatureGroupTag_ScalarDeletingDestructor(void* tag, int, unsigned int flags)
{
    CTCCreatureGroupTag_Destruct(tag);
    if (flags & 1)
        OperatorDelete(tag);
    return tag;
}