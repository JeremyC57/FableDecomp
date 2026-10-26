#pragma optimize("s", on)
extern void __fastcall CQuestInfoElementMiniGameBar_Destruct(void* bar);
void* __fastcall CQuestInfoElementMiniGameBar_ScalarDeletingDestructor(void* bar, int, unsigned int flags)
{
    CQuestInfoElementMiniGameBar_Destruct(bar);
    if (flags & 1)
        operator delete(bar);
    return bar;
}