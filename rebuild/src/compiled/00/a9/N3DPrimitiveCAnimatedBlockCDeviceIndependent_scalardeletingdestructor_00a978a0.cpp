struct CDeviceIndependent {
    void* vftable;
};

void __fastcall CDeviceIndependent_dtor(CDeviceIndependent* independent);
void operator delete(void*);

void* __fastcall CDeviceIndependent_scalar_deleting_destructor(CDeviceIndependent* independent, int edx_unused, unsigned int flags)
{
    CDeviceIndependent_dtor(independent);
    if (flags & 1)
        operator delete(independent);
    return independent;
}