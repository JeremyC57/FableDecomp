#pragma optimize("s", on)
class CAIStateGroup_EnemyCreatureBase {
public:
    virtual ~CAIStateGroup_EnemyCreatureBase();   // 0x008FD769
};
struct CAIStateGroup_HoverAttackUnlessTargetIsFleeing
{
    void* vftable;
};
extern void* const g_CAIStateGroup_HoverAttackUnlessTargetIsFleeing_vftable;
void operator delete(void*);
void* __fastcall scalar_deleting_destructor(CAIStateGroup_HoverAttackUnlessTargetIsFleeing* stateGroup, int, unsigned int flags)
{
    stateGroup->vftable = (void*)&g_CAIStateGroup_HoverAttackUnlessTargetIsFleeing_vftable;
    ((CAIStateGroup_EnemyCreatureBase*)(stateGroup))->CAIStateGroup_EnemyCreatureBase::~CAIStateGroup_EnemyCreatureBase();
    if (flags & 1)
        operator delete(stateGroup);
    return stateGroup;
}