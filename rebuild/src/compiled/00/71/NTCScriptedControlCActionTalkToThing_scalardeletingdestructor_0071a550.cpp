struct CActionTalkToThing {
    void* vftable;
};

void __fastcall CActionTalkToThing_dtor(CActionTalkToThing* action);
void operator delete(void*);

void* __fastcall CActionTalkToThing_scalar_deleting_destructor(CActionTalkToThing* action, int edx_unused, unsigned int flags)
{
    CActionTalkToThing_dtor(action);
    if (flags & 1)
        operator delete(action);
    return action;
}