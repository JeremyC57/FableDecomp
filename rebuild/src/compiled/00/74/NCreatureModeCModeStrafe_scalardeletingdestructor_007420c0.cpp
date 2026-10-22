struct CModeStrafe {
    void* vftable;
};

void __fastcall CModeStrafe_dtor(CModeStrafe* mode);
void operator delete(void*);

void* __fastcall CModeStrafe_scalar_deleting_destructor(CModeStrafe* mode, int edx_unused, unsigned int flags)
{
    CModeStrafe_dtor(mode);
    if (flags & 1)
        operator delete(mode);
    return mode;
}