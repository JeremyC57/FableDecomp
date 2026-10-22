#pragma optimize("s", on)
struct CTCCameraPointFixedDolly
{
    void* vftable;
};
extern void* const g_CTCCameraPointFixedDolly_vftable;
extern void __fastcall CTCCameraPointFixedDolly_Destruct(void* dolly);
void operator delete(void*);
void* __fastcall scalar_deleting_destructor(CTCCameraPointFixedDolly* dolly, int, unsigned int flags)
{
    dolly->vftable = (void*)&g_CTCCameraPointFixedDolly_vftable;
    CTCCameraPointFixedDolly_Destruct(dolly);
    if (flags & 1)
        operator delete(dolly);
    return dolly;
}