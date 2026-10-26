#pragma optimize("s", on)
class CTCBase {
public:
    virtual ~CTCBase();   // 0x00686830
};
void* __fastcall CTCDNone_ScalarDeletingDestructor(void* none, int, unsigned int flags)
{
    ((CTCBase*)(none))->CTCBase::~CTCBase();
    if (flags & 1)
        operator delete(none);
    return none;
}