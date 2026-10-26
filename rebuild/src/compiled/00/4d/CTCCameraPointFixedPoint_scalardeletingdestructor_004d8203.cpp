#pragma optimize("s", on)
class CTCCameraPointFixedPoint
{ public:
    void* vftable;
};
extern void* const g_CTCCameraPointFixedPoint_vftable;
extern void __fastcall CTCCameraPointFixedPoint_Destruct(void* point);
void operator delete(void*);
void* __fastcall scalar_deleting_destructor(CTCCameraPointFixedPoint* point, int, unsigned int flags)
{
    point->vftable = (void*)&g_CTCCameraPointFixedPoint_vftable;
    CTCCameraPointFixedPoint_Destruct(point);
    if (flags & 1)
        operator delete(point);
    return point;
}