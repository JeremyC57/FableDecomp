struct C3DMeshInfo {
    void* vftable;
};

void __fastcall C3DMeshInfo_dtor(C3DMeshInfo* info);
void operator delete(void*);

void* __fastcall C3DMeshInfo_scalar_deleting_destructor(C3DMeshInfo* info, int edx_unused, unsigned int flags)
{
    C3DMeshInfo_dtor(info);
    if (flags & 1)
        operator delete(info);
    return info;
}