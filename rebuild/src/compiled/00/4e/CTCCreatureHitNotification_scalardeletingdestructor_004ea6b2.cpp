#pragma optimize("s", on)
class CTCBase {
public:
    virtual ~CTCBase();   // 0x00686830
};
extern void __fastcall DestructMember_004e76fd(void* member);
void* __fastcall CTCCreatureHitNotification_CompositeScalarDeletingDestructor(void* notification, int, unsigned int flags)
{
    DestructMember_004e76fd(static_cast<unsigned char*>(notification) + 12);
    ((CTCBase*)(notification))->CTCBase::~CTCBase();
    if (flags & 1)
        operator delete(notification);
    return notification;
}