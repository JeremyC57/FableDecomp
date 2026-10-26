#pragma optimize("s", on)
class CAIStateGroupBase {
public:
    virtual ~CAIStateGroupBase();   // 0x008FD288
};
void* __fastcall CAIStateGroup_MinionReactToPlayerInteraction_ScalarDeletingDestructor(void* stateGroup, int, unsigned int flags)
{
    ((CAIStateGroupBase*)(stateGroup))->CAIStateGroupBase::~CAIStateGroupBase();
    if (flags & 1)
        operator delete(stateGroup);
    return stateGroup;
}