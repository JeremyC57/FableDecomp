struct CScriptModuleFullTransition {
    void* vftable;
};

void __fastcall CScriptModuleFullTransition_dtor(CScriptModuleFullTransition* transition);
void operator delete(void*);

void* __fastcall CScriptModuleFullTransition_scalar_deleting_destructor(CScriptModuleFullTransition* transition, int edx_unused, unsigned int flags)
{
    CScriptModuleFullTransition_dtor(transition);
    if (flags & 1)
        operator delete(transition);
    return transition;
}