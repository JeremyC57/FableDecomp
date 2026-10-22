struct CEngineInternalPrimitiveChanger {
    void* vftable;
};

void __fastcall CEngineInternalPrimitiveChanger_dtor(CEngineInternalPrimitiveChanger* changer);
void operator delete(void*);

void* __fastcall CEngineInternalPrimitiveChanger_scalar_deleting_destructor(CEngineInternalPrimitiveChanger* changer, int edx_unused, unsigned int flags)
{
    CEngineInternalPrimitiveChanger_dtor(changer);
    if (flags & 1)
        operator delete(changer);
    return changer;
}