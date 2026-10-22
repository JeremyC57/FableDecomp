struct CEngineInternalAttachedPrimitive {
    void* vftable;
};

void __fastcall CEngineInternalAttachedPrimitive_dtor(CEngineInternalAttachedPrimitive* primitive);
void operator delete(void*);

void* __fastcall CEngineInternalAttachedPrimitive_scalar_deleting_destructor(CEngineInternalAttachedPrimitive* primitive, int edx_unused, unsigned int flags)
{
    CEngineInternalAttachedPrimitive_dtor(primitive);
    if (flags & 1)
        operator delete(primitive);
    return primitive;
}