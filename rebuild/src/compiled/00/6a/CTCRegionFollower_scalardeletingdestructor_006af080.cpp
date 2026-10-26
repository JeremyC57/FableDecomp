#pragma optimize("t", on)
class CTCBase {
public:
    virtual ~CTCBase();   // 0x00686830
};
extern void __fastcall DestructMember_004b96f0(void* member);
void* __fastcall CTCRegionFollower_CompositeScalarDeletingDestructor(void* follower, int, unsigned int flags)
{
    DestructMember_004b96f0(static_cast<unsigned char*>(follower) + 12);
    ((CTCBase*)(follower))->CTCBase::~CTCBase();
    if (flags & 1)
        operator delete(follower);
    return follower;
}