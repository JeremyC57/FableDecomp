#pragma optimize("s", on)
class CTCBase {
public:
    virtual ~CTCBase();   // 0x00686830
};
extern void __fastcall DestructMember_004b8940(void* member);
void* __fastcall CTCParticleAttachmentTracker_CompositeScalarDeletingDestructor(void* tracker, int, unsigned int flags)
{
    DestructMember_004b8940(static_cast<unsigned char*>(tracker) + 12);
    ((CTCBase*)(tracker))->CTCBase::~CTCBase();
    if (flags & 1)
        operator delete(tracker);
    return tracker;
}