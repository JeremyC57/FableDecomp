struct CTCQuestCompletionUI {
    void* vftable;
};

void __fastcall CTCQuestCompletionUI_dtor(CTCQuestCompletionUI* completionUI);
void operator delete(void*);

void* __fastcall CTCQuestCompletionUI_scalar_deleting_destructor(CTCQuestCompletionUI* completionUI, int edx_unused, unsigned int flags)
{
    CTCQuestCompletionUI_dtor(completionUI);
    if (flags & 1)
        operator delete(completionUI);
    return completionUI;
}