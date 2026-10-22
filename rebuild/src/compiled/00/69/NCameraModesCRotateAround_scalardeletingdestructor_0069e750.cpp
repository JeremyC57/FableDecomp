struct CRotateAround {
    void* vftable;
};

void __fastcall CRotateAround_dtor(CRotateAround* around);
void operator delete(void*);

void* __fastcall CRotateAround_scalar_deleting_destructor(CRotateAround* around, int edx_unused, unsigned int flags)
{
    CRotateAround_dtor(around);
    if (flags & 1)
        operator delete(around);
    return around;
}