struct CModeBlock {
    void* vftable;
};

void __fastcall CModeBlock_dtor(CModeBlock* mode);
void operator delete(void*);

void* __fastcall CModeBlock_scalar_deleting_destructor(CModeBlock* mode, int edx_unused, unsigned int flags)
{
    CModeBlock_dtor(mode);
    if (flags & 1)
        operator delete(mode);
    return mode;
}