class CQuestManager { public:
    void* vftable;
};

void __fastcall CQuestManager_dtor(CQuestManager* manager);
void operator delete(void*);

void* __fastcall CQuestManager_scalar_deleting_destructor(CQuestManager* manager, int edx_unused, unsigned int flags)
{
    CQuestManager_dtor(manager);
    if (flags & 1)
        operator delete(manager);
    return manager;
}