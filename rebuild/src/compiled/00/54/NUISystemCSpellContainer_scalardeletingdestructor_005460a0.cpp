struct CSpellContainer {
    void* vftable;
};

void __fastcall CSpellContainer_dtor(CSpellContainer* container);
void operator delete(void*);

void* __fastcall CSpellContainer_scalar_deleting_destructor(CSpellContainer* container, int edx_unused, unsigned int flags)
{
    CSpellContainer_dtor(container);
    if (flags & 1)
        operator delete(container);
    return container;
}