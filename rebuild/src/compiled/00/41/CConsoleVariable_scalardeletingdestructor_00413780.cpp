struct CInner;
// Leaf: `scalar_deleting_destructor'
void __fastcall CConsoleVariable_dtor_inner(CInner* variable);

struct CConsoleVariable {
    unsigned int vftable;
    char inner[0xb4];
    void* tex;
};

void* __fastcall CConsoleVariable_scalar_deleting_destructor(CConsoleVariable* variable, void* edx, unsigned int flags)
{
    (void)edx;
    variable->vftable = 0x122e5b0;
    CConsoleVariable_dtor_inner((CInner*)(&variable->inner));
    if (flags & 1)
        operator delete(variable);
    return variable;
}