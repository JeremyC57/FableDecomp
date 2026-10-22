#pragma optimize("s", on)
struct CTCCameraPointGeneralCase
{
    void* vftable;
};
extern void* const g_CTCCameraPointGeneralCase_vftable;
extern void __fastcall CTCCameraPointGeneralCase_Destruct(void* generalCase);
void operator delete(void*);
void* __fastcall scalar_deleting_destructor(CTCCameraPointGeneralCase* generalCase, int, unsigned int flags)
{
    generalCase->vftable = (void*)&g_CTCCameraPointGeneralCase_vftable;
    CTCCameraPointGeneralCase_Destruct(generalCase);
    if (flags & 1)
        operator delete(generalCase);
    return generalCase;
}