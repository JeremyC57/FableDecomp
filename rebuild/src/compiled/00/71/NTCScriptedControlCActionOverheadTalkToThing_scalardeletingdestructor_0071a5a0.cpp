struct CActionOverheadTalkToThing {
    void* vftable;
};

void __fastcall CActionOverheadTalkToThing_dtor(CActionOverheadTalkToThing* action);
void operator delete(void*);

void* __fastcall CActionOverheadTalkToThing_scalar_deleting_destructor(CActionOverheadTalkToThing* action, int edx_unused, unsigned int flags)
{
    CActionOverheadTalkToThing_dtor(action);
    if (flags & 1)
        operator delete(action);
    return action;
}