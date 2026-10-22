struct CScriptModuleSpline {
    void* vftable;
};

void __fastcall CScriptModuleSpline_dtor(CScriptModuleSpline* spline);
void operator delete(void*);

void* __fastcall CScriptModuleSpline_scalar_deleting_destructor(CScriptModuleSpline* spline, int edx_unused, unsigned int flags)
{
    CScriptModuleSpline_dtor(spline);
    if (flags & 1)
        operator delete(spline);
    return spline;
}