struct CTCGraphicAppearance {
    void* vftable;
};

void __fastcall CTCGraphicAppearance_dtor(CTCGraphicAppearance* appearance);
void operator delete(void*);

void* __fastcall CTCGraphicAppearance_scalar_deleting_destructor(CTCGraphicAppearance* appearance, int edx_unused, unsigned int flags)
{
    CTCGraphicAppearance_dtor(appearance);
    if (flags & 1)
        operator delete(appearance);
    return appearance;
}