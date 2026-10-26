class CCreatureActionBase { public:
    void* vftable;

public:
    virtual ~CCreatureActionBase();   // 0x00693EF0
};

void operator delete(void*);

void* __fastcall CCreatureActionBase_scalar_deleting_destructor(CCreatureActionBase* action, int edx_unused, unsigned int flags)
{
    ((CCreatureActionBase*)(action))->CCreatureActionBase::~CCreatureActionBase();
    if (flags & 1)
        operator delete(action);
    return action;
}