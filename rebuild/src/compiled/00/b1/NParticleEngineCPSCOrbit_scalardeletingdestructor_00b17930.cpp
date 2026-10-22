struct CPSCOrbit {
    void* vftable;
};

void __fastcall CPSCOrbit_dtor(CPSCOrbit* orbit);
void operator delete(void*);

void* __fastcall CPSCOrbit_scalar_deleting_destructor(CPSCOrbit* orbit, int edx_unused, unsigned int flags)
{
    CPSCOrbit_dtor(orbit);
    if (flags & 1)
        operator delete(orbit);
    return orbit;
}