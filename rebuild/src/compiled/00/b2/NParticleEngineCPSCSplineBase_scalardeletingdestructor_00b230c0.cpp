struct CPSCSplineBase {
    void* vftable;
};

void __fastcall CPSCSplineBase_dtor(CPSCSplineBase* spline);
void operator delete(void*);

void* __fastcall CPSCSplineBase_scalar_deleting_destructor(CPSCSplineBase* spline, int edx_unused, unsigned int flags)
{
    CPSCSplineBase_dtor(spline);
    if (flags & 1)
        operator delete(spline);
    return spline;
}