#pragma optimize("t", on)
class CTCBase {
public:
    virtual ~CTCBase();   // 0x00686830
};
extern void __fastcall DestructFirstMember_005142e0(void* member);
extern void __fastcall DestructSecondMember_004b96f0(void* member);
void* __fastcall CTCFollowed_CompositeScalarDeletingDestructor(void* followed, int, unsigned int flags)
{
    DestructFirstMember_005142e0(static_cast<unsigned char*>(followed) + 16);
    DestructSecondMember_004b96f0(static_cast<unsigned char*>(followed) + 12);
    ((CTCBase*)(followed))->CTCBase::~CTCBase();
    if (flags & 1)
        operator delete(followed);
    return followed;
}