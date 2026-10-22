struct CPSCLight {
    void* vftable;
};

void __fastcall CPSCLight_dtor(CPSCLight* light);
void operator delete(void*);

void* __fastcall CPSCLight_scalar_deleting_destructor(CPSCLight* light, int edx_unused, unsigned int flags)
{
    CPSCLight_dtor(light);
    if (flags & 1)
        operator delete(light);
    return light;
}