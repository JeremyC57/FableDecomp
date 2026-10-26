#pragma optimize("s", on)
class CTCBase {
public:
    virtual ~CTCBase();   // 0x00686830
};
extern void __fastcall DestructMember_004e1bbb(void* member);
void* __fastcall CTCEntityEvents_CompositeScalarDeletingDestructor(void* events, int, unsigned int flags)
{
    DestructMember_004e1bbb(static_cast<unsigned char*>(events) + 12);
    ((CTCBase*)(events))->CTCBase::~CTCBase();
    if (flags & 1)
        operator delete(events);
    return events;
}