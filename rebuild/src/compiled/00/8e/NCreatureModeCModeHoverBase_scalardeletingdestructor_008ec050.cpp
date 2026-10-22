struct CModeHoverBase {
    void* vftable;
};

void __fastcall CModeHoverBase_dtor(CModeHoverBase* mode);
void operator delete(void*);

void* __fastcall CModeHoverBase_scalar_deleting_destructor(CModeHoverBase* mode, int edx_unused, unsigned int flags)
{
    CModeHoverBase_dtor(mode);
    if (flags & 1)
        operator delete(mode);
    return mode;
}