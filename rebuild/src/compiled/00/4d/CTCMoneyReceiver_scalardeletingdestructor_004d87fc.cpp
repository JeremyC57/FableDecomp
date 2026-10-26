#pragma optimize("s", on)
class CTCBase {
public:
    virtual ~CTCBase();   // 0x00686830
};
void* __fastcall CTCMoneyReceiver_ScalarDeletingDestructor(void* receiver, int, unsigned int flags)
{
    ((CTCBase*)(receiver))->CTCBase::~CTCBase();
    if (flags & 1)
        operator delete(receiver);
    return receiver;
}