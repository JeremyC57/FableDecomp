#pragma optimize("s", on)
class CTCBase {
public:
    virtual ~CTCBase();   // 0x00686830
};
class CTCCameraPointFixedPoint
{ public:
    void* vftable;
};
extern void* const g_CTCCameraPointFixedPoint_vftable;
void operator delete(void*);
void* __fastcall scalar_deleting_destructor(CTCCameraPointFixedPoint* point, int, unsigned int flags)
{
    point->vftable = (void*)&g_CTCCameraPointFixedPoint_vftable;
    ((CTCBase*)(point))->CTCBase::~CTCBase();
    if (flags & 1)
        operator delete(point);
    return point;
}