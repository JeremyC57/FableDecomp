struct CCreatureAction_PlayConversationAnimation {
    void* vftable;
};

void __fastcall CCreatureAction_PlayConversationAnimation_dtor(CCreatureAction_PlayConversationAnimation* action);
void operator delete(void*);

void* __fastcall CCreatureAction_PlayConversationAnimation_scalar_deleting_destructor(CCreatureAction_PlayConversationAnimation* action, int edx_unused, unsigned int flags)
{
    CCreatureAction_PlayConversationAnimation_dtor(action);
    if (flags & 1)
        operator delete(action);
    return action;
}