class C3DMeshLODInfo { public:
    void* vftable;
};

void __fastcall C3DMeshLODInfo_dtor(C3DMeshLODInfo* info);
void operator delete(void*);

void* __fastcall C3DMeshLODInfo_scalar_deleting_destructor(C3DMeshLODInfo* info, int edx_unused, unsigned int flags)
{
    C3DMeshLODInfo_dtor(info);
    if (flags & 1)
        operator delete(info);
    return info;
}