struct CEnginePrimitiveOcclusionInfo {
    void* vftable;
};

void __fastcall CEnginePrimitiveOcclusionInfo_dtor(CEnginePrimitiveOcclusionInfo* info);
void operator delete(void*);

void* __fastcall CEnginePrimitiveOcclusionInfo_scalar_deleting_destructor(CEnginePrimitiveOcclusionInfo* info, int edx_unused, unsigned int flags)
{
    CEnginePrimitiveOcclusionInfo_dtor(info);
    if (flags & 1)
        operator delete(info);
    return info;
}