#pragma optimize("s", on)
class CTCBase {
public:
    virtual ~CTCBase();   // 0x00686830
};
void* __fastcall CTCDCameraPoint_ScalarDeletingDestructor(void* point, int, unsigned int flags)
{
    ((CTCBase*)(point))->CTCBase::~CTCBase();
    if (flags & 1)
        operator delete(point);
    return point;
}