class CCombatActionBase {
public:
    virtual ~CCombatActionBase();   // 0x0062E900
};
class CCreatureAction_DiagonalStrafeBR { public:
    void* vftable;
};

void operator delete(void*);

void* __fastcall CCreatureAction_DiagonalStrafeBR_scalar_deleting_destructor(CCreatureAction_DiagonalStrafeBR* action, int edx_unused, unsigned int flags)
{
    ((CCombatActionBase*)(action))->CCombatActionBase::~CCombatActionBase();
    if (flags & 1)
        operator delete(action);
    return action;
}