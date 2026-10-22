#pragma optimize("s",on)
struct CCameraModeDef {
    int f0;
    void* vf;
};
extern void __fastcall CCameraModeDef_dtor(CCameraModeDef* mode);
extern void __cdecl operator_delete_stub(void* p);
void* __fastcall CCameraModeDef_scalar_deleting_destructor(CCameraModeDef* mode, int edx_unused, unsigned int flags)
{
    CCameraModeDef_dtor(mode);
    if (flags & 1)
        operator_delete_stub(mode);
    return mode;
}