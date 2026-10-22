struct CSharedData {
    void* vftable;
};

void __fastcall CSharedData_dtor(CSharedData* shared);
void operator delete(void*);

void* __fastcall CSharedData_scalar_deleting_destructor(CSharedData* shared, int edx_unused, unsigned int flags)
{
    CSharedData_dtor(shared);
    if (flags & 1)
        operator delete(shared);
    return shared;
}