#pragma optimize("s", on)
struct CTCCameraPointDead
{
    void* vftable;
};
extern void* const g_CTCCameraPointDead_vftable;
extern void __fastcall CTCCameraPointDead_Destruct(void* dead);
void operator delete(void*);
void* __fastcall scalar_deleting_destructor(CTCCameraPointDead* dead, int, unsigned int flags)
{
    dead->vftable = (void*)&g_CTCCameraPointDead_vftable;
    CTCCameraPointDead_Destruct(dead);
    if (flags & 1)
        operator delete(dead);
    return dead;
}