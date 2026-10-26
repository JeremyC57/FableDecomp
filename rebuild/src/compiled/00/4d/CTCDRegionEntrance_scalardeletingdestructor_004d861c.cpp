#pragma optimize("s", on)
class CTCBase {
public:
    virtual ~CTCBase();   // 0x00686830
};
void* __fastcall CTCDRegionEntrance_ScalarDeletingDestructor(void* entrance, int, unsigned int flags)
{
    ((CTCBase*)(entrance))->CTCBase::~CTCBase();
    if (flags & 1)
        operator delete(entrance);
    return entrance;
}