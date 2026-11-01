#pragma optimize("t", on)
class CCombatActionBase {
public:
    virtual ~CCombatActionBase();   // 0x0062E900
};
class CCharString { public:
    ~CCharString();   // 0x0099EAE0 (Ego_r.pdb ??1CCharString@@QAE@XZ)
};
void* __fastcall CCreatureAction_TrollProjectileAttack_CompositeScalarDeletingDestructor(void* action, int, unsigned int flags)
{
    ((CCharString*)(static_cast<unsigned char*>(action) + 280))->~CCharString();
    ((CCombatActionBase*)(action))->CCombatActionBase::~CCombatActionBase();
    if (flags & 1)
        operator delete(action);
    return action;
}