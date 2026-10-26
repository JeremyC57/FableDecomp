#pragma optimize("t", on)
class CCombatActionBase {
public:
    virtual ~CCombatActionBase();   // 0x0062E900
};
extern void __fastcall DestructMember_0048c5c0(void* member);
void* __fastcall CCreatureAction_CastAssassinRush_CompositeScalarDeletingDestructor(void* action, int, unsigned int flags)
{
    DestructMember_0048c5c0(static_cast<unsigned char*>(action) + 300);
    ((CCombatActionBase*)(action))->CCombatActionBase::~CCombatActionBase();
    if (flags & 1)
        operator delete(action);
    return action;
}