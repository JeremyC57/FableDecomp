#pragma optimize("s", on)
extern void __fastcall CQuestInfoElementMiniGameBar_Destruct(void* bar);
extern void __cdecl OperatorDelete(void* object);
void* __fastcall CQuestInfoElementMiniGameBar_ScalarDeletingDestructor(void* bar, int, unsigned int flags)
{
    CQuestInfoElementMiniGameBar_Destruct(bar);
    if (flags & 1)
        OperatorDelete(bar);
    return bar;
}