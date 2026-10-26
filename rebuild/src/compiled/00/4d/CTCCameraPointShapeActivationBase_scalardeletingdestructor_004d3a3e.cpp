#pragma optimize("s", on)
class CTCBase {
public:
    virtual ~CTCBase();   // 0x00686830
};
struct CTCCameraPointShapeActivationBase
{
    void* vftable;
};
extern void* const g_CTCCameraPointShapeActivationBase_vftable;
void operator delete(void*);
void* __fastcall scalar_deleting_destructor(CTCCameraPointShapeActivationBase* activation, int, unsigned int flags)
{
    activation->vftable = (void*)&g_CTCCameraPointShapeActivationBase_vftable;
    ((CTCBase*)(activation))->CTCBase::~CTCBase();
    if (flags & 1)
        operator delete(activation);
    return activation;
}