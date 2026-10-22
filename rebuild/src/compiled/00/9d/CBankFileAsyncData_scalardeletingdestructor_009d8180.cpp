struct CBankFileAsyncData {
    void* vftable;
};

void __fastcall CBankFileAsyncData_dtor(CBankFileAsyncData* async);
void operator delete(void*);

void* __fastcall CBankFileAsyncData_scalar_deleting_destructor(CBankFileAsyncData* async, int edx_unused, unsigned int flags)
{
    CBankFileAsyncData_dtor(async);
    if (flags & 1)
        operator delete(async);
    return async;
}