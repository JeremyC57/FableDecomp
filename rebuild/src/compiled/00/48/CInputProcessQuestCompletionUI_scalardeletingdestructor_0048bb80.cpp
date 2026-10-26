class CInputProcessQuestCompletionUI { public:
    void* vftable;
};

void __fastcall CInputProcessQuestCompletionUI_dtor(CInputProcessQuestCompletionUI* process);
void operator delete(void*);

void* __fastcall CInputProcessQuestCompletionUI_scalar_deleting_destructor(CInputProcessQuestCompletionUI* process, int edx_unused, unsigned int flags)
{
    CInputProcessQuestCompletionUI_dtor(process);
    if (flags & 1)
        operator delete(process);
    return process;
}