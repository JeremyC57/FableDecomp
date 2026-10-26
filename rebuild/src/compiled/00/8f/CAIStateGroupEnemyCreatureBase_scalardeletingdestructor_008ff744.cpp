#pragma optimize("s", on)
class CAIStateGroup_EnemyCreatureBase {
public:
    virtual ~CAIStateGroup_EnemyCreatureBase();   // 0x008FD769
};
void* __fastcall CAIStateGroup_EnemyCreatureBase_ScalarDeletingDestructor(void* stateGroup, int, unsigned int flags)
{
    ((CAIStateGroup_EnemyCreatureBase*)(stateGroup))->CAIStateGroup_EnemyCreatureBase::~CAIStateGroup_EnemyCreatureBase();
    if (flags & 1)
        operator delete(stateGroup);
    return stateGroup;
}