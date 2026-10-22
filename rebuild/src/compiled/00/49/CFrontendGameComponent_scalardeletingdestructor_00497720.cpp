struct CFrontendGameComponent {
    void* vftable;
};

void __fastcall CFrontendGameComponent_dtor(CFrontendGameComponent* component);
void operator delete(void*);

void* __fastcall CFrontendGameComponent_scalar_deleting_destructor(CFrontendGameComponent* component, int edx_unused, unsigned int flags)
{
    CFrontendGameComponent_dtor(component);
    if (flags & 1)
        operator delete(component);
    return component;
}