#pragma optimize("s", on)
class CTCBase {
public:
    virtual ~CTCBase();   // 0x00686830
};
class CTCCameraPointFixedDolly
{ public:
    void* vftable;
};
extern void* const g_CTCCameraPointFixedDolly_vftable;
void operator delete(void*);
void* __fastcall scalar_deleting_destructor(CTCCameraPointFixedDolly* dolly, int, unsigned int flags)
{
    dolly->vftable = (void*)&g_CTCCameraPointFixedDolly_vftable;
    ((CTCBase*)(dolly))->CTCBase::~CTCBase();
    if (flags & 1)
        operator delete(dolly);
    return dolly;
}