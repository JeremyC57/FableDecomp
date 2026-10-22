#pragma optimize("s", on)
struct CTCCameraPointShapeActivationBase
{
    void* vftable;
};
extern void* const g_CTCCameraPointShapeActivationBase_vftable;
extern void __fastcall CTCCameraPointShapeActivationBase_Destruct(void* activation);
void operator delete(void*);
void* __fastcall scalar_deleting_destructor(CTCCameraPointShapeActivationBase* activation, int, unsigned int flags)
{
    activation->vftable = (void*)&g_CTCCameraPointShapeActivationBase_vftable;
    CTCCameraPointShapeActivationBase_Destruct(activation);
    if (flags & 1)
        operator delete(activation);
    return activation;
}