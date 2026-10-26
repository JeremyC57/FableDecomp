#pragma optimize("t", on)
class CCombatActionBase {
public:
    virtual ~CCombatActionBase();   // 0x0062E900
};
extern void __fastcall DestructMember_0099eae0(void* member);
void* __fastcall CCreatureAction_TrollProjectileAttack_CompositeScalarDeletingDestructor(void* action, int, unsigned int flags)
{
    DestructMember_0099eae0(static_cast<unsigned char*>(action) + 280);
    ((CCombatActionBase*)(action))->CCombatActionBase::~CCombatActionBase();
    if (flags & 1)
        operator delete(action);
    return action;
}