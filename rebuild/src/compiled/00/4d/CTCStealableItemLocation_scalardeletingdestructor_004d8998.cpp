#pragma optimize("s", on)
class CTCBase {
public:
    virtual ~CTCBase();   // 0x00686830
};
void* __fastcall CTCStealableItemLocation_ScalarDeletingDestructor(void* location, int, unsigned int flags)
{
    ((CTCBase*)(location))->CTCBase::~CTCBase();
    if (flags & 1)
        operator delete(location);
    return location;
}