struct CGroundNavigatorExternalInterface {
    void* vftable;
};

void __fastcall CGroundNavigatorExternalInterface_dtor(CGroundNavigatorExternalInterface* interface);
void operator delete(void*);

void* __fastcall CGroundNavigatorExternalInterface_scalar_deleting_destructor(CGroundNavigatorExternalInterface* interface, int edx_unused, unsigned int flags)
{
    CGroundNavigatorExternalInterface_dtor(interface);
    if (flags & 1)
        operator delete(interface);
    return interface;
}