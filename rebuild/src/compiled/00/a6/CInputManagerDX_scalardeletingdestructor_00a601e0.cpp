struct CInputManagerDX {
    void* vftable;
};

void __fastcall CInputManagerDX_dtor(CInputManagerDX* managerDX);
void operator delete(void*);

void* __fastcall CInputManagerDX_scalar_deleting_destructor(CInputManagerDX* managerDX, int edx_unused, unsigned int flags)
{
    CInputManagerDX_dtor(managerDX);
    if (flags & 1)
        operator delete(managerDX);
    return managerDX;
}