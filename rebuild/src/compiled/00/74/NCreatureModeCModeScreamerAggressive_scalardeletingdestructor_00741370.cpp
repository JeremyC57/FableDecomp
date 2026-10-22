struct CModeScreamerAggressive {
    void* vftable;
};

void __fastcall CModeScreamerAggressive_dtor(CModeScreamerAggressive* mode);
void operator delete(void*);

void* __fastcall CModeScreamerAggressive_scalar_deleting_destructor(CModeScreamerAggressive* mode, int edx_unused, unsigned int flags)
{
    CModeScreamerAggressive_dtor(mode);
    if (flags & 1)
        operator delete(mode);
    return mode;
}