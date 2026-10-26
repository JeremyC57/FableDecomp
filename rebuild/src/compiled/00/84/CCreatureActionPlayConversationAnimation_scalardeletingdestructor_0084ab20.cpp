class CCreatureActionBase {
public:
    virtual ~CCreatureActionBase();   // 0x00693EF0
};
class CCreatureAction_PlayConversationAnimation { public:
    void* vftable;
};

void operator delete(void*);

void* __fastcall CCreatureAction_PlayConversationAnimation_scalar_deleting_destructor(CCreatureAction_PlayConversationAnimation* action, int edx_unused, unsigned int flags)
{
    ((CCreatureActionBase*)(action))->CCreatureActionBase::~CCreatureActionBase();
    if (flags & 1)
        operator delete(action);
    return action;
}