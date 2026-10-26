#pragma optimize("s", on)
class CTCBase {
public:
    virtual ~CTCBase();   // 0x00686830
};
void* __fastcall CTCWobble_ScalarDeletingDestructor(void* wobble, int, unsigned int flags)
{
    ((CTCBase*)(wobble))->CTCBase::~CTCBase();
    if (flags & 1)
        operator delete(wobble);
    return wobble;
}