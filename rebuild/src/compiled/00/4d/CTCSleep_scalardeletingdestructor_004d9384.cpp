#pragma optimize("s", on)
class CTCBase {
public:
    virtual ~CTCBase();   // 0x00686830
};
void* __fastcall CTCSleep_ScalarDeletingDestructor(void* sleep, int, unsigned int flags)
{
    ((CTCBase*)(sleep))->CTCBase::~CTCBase();
    if (flags & 1)
        operator delete(sleep);
    return sleep;
}