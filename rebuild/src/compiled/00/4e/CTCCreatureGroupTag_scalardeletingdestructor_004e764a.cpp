#pragma optimize("s", on)
extern void __fastcall CTCCreatureGroupTag_Destruct(void* tag);
void* __fastcall CTCCreatureGroupTag_ScalarDeletingDestructor(void* tag, int, unsigned int flags)
{
    CTCCreatureGroupTag_Destruct(tag);
    if (flags & 1)
        operator delete(tag);
    return tag;
}