struct CModeOutOfActionBase {
    void* vftable;
};

void __fastcall CModeOutOfActionBase_dtor(CModeOutOfActionBase* mode);
void operator delete(void*);

void* __fastcall CModeOutOfActionBase_scalar_deleting_destructor(CModeOutOfActionBase* mode, int edx_unused, unsigned int flags)
{
    CModeOutOfActionBase_dtor(mode);
    if (flags & 1)
        operator delete(mode);
    return mode;
}