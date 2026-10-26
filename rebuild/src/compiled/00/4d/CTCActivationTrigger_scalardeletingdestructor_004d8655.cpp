#pragma optimize("s", on)
class CTCBase {
public:
    virtual ~CTCBase();   // 0x00686830
};
void* __fastcall CTCActivationTrigger_ScalarDeletingDestructor(void* trigger, int, unsigned int flags)
{
    ((CTCBase*)(trigger))->CTCBase::~CTCBase();
    if (flags & 1)
        operator delete(trigger);
    return trigger;
}