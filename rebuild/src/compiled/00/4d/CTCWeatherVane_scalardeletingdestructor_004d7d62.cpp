#pragma optimize("s", on)
class CTCBase {
public:
    virtual ~CTCBase();   // 0x00686830
};
void* __fastcall CTCWeatherVane_ScalarDeletingDestructor(void* vane, int, unsigned int flags)
{
    ((CTCBase*)(vane))->CTCBase::~CTCBase();
    if (flags & 1)
        operator delete(vane);
    return vane;
}