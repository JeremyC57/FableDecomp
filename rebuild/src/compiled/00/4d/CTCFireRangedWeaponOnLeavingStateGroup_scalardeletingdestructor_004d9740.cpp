#pragma optimize("s", on)
class CTCBase {
public:
    virtual ~CTCBase();   // 0x00686830
};
void* __fastcall CTCFireRangedWeaponOnLeavingStateGroup_ScalarDeletingDestructor(void* group, int, unsigned int flags)
{
    ((CTCBase*)(group))->CTCBase::~CTCBase();
    if (flags & 1)
        operator delete(group);
    return group;
}