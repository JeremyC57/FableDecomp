struct CScriptModuleDolly {
    void* vftable;
};

void __fastcall CScriptModuleDolly_dtor(CScriptModuleDolly* dolly);
void operator delete(void*);

void* __fastcall CScriptModuleDolly_scalar_deleting_destructor(CScriptModuleDolly* dolly, int edx_unused, unsigned int flags)
{
    CScriptModuleDolly_dtor(dolly);
    if (flags & 1)
        operator delete(dolly);
    return dolly;
}