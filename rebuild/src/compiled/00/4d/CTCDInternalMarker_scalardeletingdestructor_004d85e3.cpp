#pragma optimize("s", on)
class CTCBase {
public:
    virtual ~CTCBase();   // 0x00686830
};
void* __fastcall CTCDInternalMarker_ScalarDeletingDestructor(void* marker, int, unsigned int flags)
{
    ((CTCBase*)(marker))->CTCBase::~CTCBase();
    if (flags & 1)
        operator delete(marker);
    return marker;
}