class CCombatActionBase {
public:
    virtual ~CCombatActionBase();   // 0x0062E900
};
class CCreatureAction_DuckAndCover { public:
    void* vftable;
};

void operator delete(void*);

void* __fastcall CCreatureAction_DuckAndCover_scalar_deleting_destructor(CCreatureAction_DuckAndCover* action, int edx_unused, unsigned int flags)
{
    ((CCombatActionBase*)(action))->CCombatActionBase::~CCombatActionBase();
    if (flags & 1)
        operator delete(action);
    return action;
}