struct CModeStandBase {
    void* vftable;
};

void __fastcall CModeStandBase_dtor(CModeStandBase* mode);
void operator delete(void*);

void* __fastcall CModeStandBase_scalar_deleting_destructor(CModeStandBase* mode, int edx_unused, unsigned int flags)
{
    CModeStandBase_dtor(mode);
    if (flags & 1)
        operator delete(mode);
    return mode;
}