struct CTCShotDefinitionBase {
    void* vftable;
};

void __fastcall CTCShotDefinitionBase_dtor(CTCShotDefinitionBase* definition);
void operator delete(void*);

void* __fastcall CTCShotDefinitionBase_scalar_deleting_destructor(CTCShotDefinitionBase* definition, int edx_unused, unsigned int flags)
{
    CTCShotDefinitionBase_dtor(definition);
    if (flags & 1)
        operator delete(definition);
    return definition;
}