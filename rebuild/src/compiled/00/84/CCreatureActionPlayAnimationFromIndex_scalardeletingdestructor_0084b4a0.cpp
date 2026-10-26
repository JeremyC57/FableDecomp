class CCreatureActionBase {
public:
    virtual ~CCreatureActionBase();   // 0x00693EF0
};
class CCreatureAction_PlayAnimationFromIndex { public:
    void* vftable;
};

void operator delete(void*);

void* __fastcall CCreatureAction_PlayAnimationFromIndex_scalar_deleting_destructor(CCreatureAction_PlayAnimationFromIndex* action, int edx_unused, unsigned int flags)
{
    ((CCreatureActionBase*)(action))->CCreatureActionBase::~CCreatureActionBase();
    if (flags & 1)
        operator delete(action);
    return action;
}