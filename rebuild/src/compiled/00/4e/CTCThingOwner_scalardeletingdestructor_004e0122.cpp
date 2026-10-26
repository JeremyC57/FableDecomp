#pragma optimize("s", on)
class CTCBase {
public:
    virtual ~CTCBase();   // 0x00686830
};
extern void __fastcall DestructMember_004b8940(void* member);
void* __fastcall CTCThingOwner_CompositeScalarDeletingDestructor(void* owner, int, unsigned int flags)
{
    DestructMember_004b8940(static_cast<unsigned char*>(owner) + 12);
    ((CTCBase*)(owner))->CTCBase::~CTCBase();
    if (flags & 1)
        operator delete(owner);
    return owner;
}