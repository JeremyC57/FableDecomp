struct CTCCameraPointScripted {
    void* vftable;
};

void __fastcall CTCCameraPointScripted_dtor(CTCCameraPointScripted* scripted);
void operator delete(void*);

void* __fastcall CTCCameraPointScripted_scalar_deleting_destructor(CTCCameraPointScripted* scripted, int edx_unused, unsigned int flags)
{
    CTCCameraPointScripted_dtor(scripted);
    if (flags & 1)
        operator delete(scripted);
    return scripted;
}