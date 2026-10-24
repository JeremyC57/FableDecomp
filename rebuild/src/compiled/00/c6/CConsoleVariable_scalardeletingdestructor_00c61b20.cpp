struct CConsoleVariable {
// Leaf: `scalar_deleting_destructor'
    void* vftable;
};

void __fastcall CConsoleVariable_dtor(CConsoleVariable* variable);
extern void free_engine(void* p);

void* __fastcall CConsoleVariable_scalar_deleting_destructor(CConsoleVariable* variable, int edx_unused, unsigned int flags)
{
    CConsoleVariable_dtor(variable);
    if (flags & 1)
        free_engine(variable);
    return variable;
}