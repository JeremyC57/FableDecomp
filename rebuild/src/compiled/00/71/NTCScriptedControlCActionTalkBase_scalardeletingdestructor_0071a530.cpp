struct CActionTalkBase {
    void* vftable;
};

void __fastcall CActionTalkBase_dtor(CActionTalkBase* action);
void operator delete(void*);

void* __fastcall CActionTalkBase_scalar_deleting_destructor(CActionTalkBase* action, int edx_unused, unsigned int flags)
{
    CActionTalkBase_dtor(action);
    if (flags & 1)
        operator delete(action);
    return action;
}