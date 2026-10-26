#pragma optimize("s", on)
class CTCBase {
public:
    virtual ~CTCBase();   // 0x00686830
};
void* __fastcall CTCDestroyThingOnSpecialAbilityFinish_ScalarDeletingDestructor(void* finish, int, unsigned int flags)
{
    ((CTCBase*)(finish))->CTCBase::~CTCBase();
    if (flags & 1)
        operator delete(finish);
    return finish;
}