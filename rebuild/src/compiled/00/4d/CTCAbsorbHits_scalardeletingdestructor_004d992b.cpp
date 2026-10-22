#pragma optimize("s", on)
extern void __fastcall CTCAbsorbHits_Destruct(void* hits);
extern void __cdecl OperatorDelete(void* object);
void* __fastcall CTCAbsorbHits_ScalarDeletingDestructor(void* hits, int, unsigned int flags)
{
    CTCAbsorbHits_Destruct(hits);
    if (flags & 1)
        OperatorDelete(hits);
    return hits;
}