#pragma optimize("s", on)
class CAIStateGroup_EnemyCreatureBase {
public:
    virtual ~CAIStateGroup_EnemyCreatureBase();   // 0x008FD769
};
extern void __fastcall DestructMember_004406d0(void* member);
void* __fastcall CAIStateGroup_WhisperPotionCombat_CompositeScalarDeletingDestructor(void* stateGroup, int, unsigned int flags)
{
    DestructMember_004406d0(static_cast<unsigned char*>(stateGroup) + 28);
    ((CAIStateGroup_EnemyCreatureBase*)(stateGroup))->CAIStateGroup_EnemyCreatureBase::~CAIStateGroup_EnemyCreatureBase();
    if (flags & 1)
        operator delete(stateGroup);
    return stateGroup;
}