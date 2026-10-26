#pragma optimize("s", on)
class CParentDefClassBase {
public:
    virtual ~CParentDefClassBase();   // 0x00430300
};
void* __fastcall CMeleeCombatKnockdownEffects_ScalarDeletingDestructor(void* effects, int, unsigned int flags)
{
    ((CParentDefClassBase*)(effects))->CParentDefClassBase::~CParentDefClassBase();
    if (flags & 1)
        operator delete(effects);
    return effects;
}