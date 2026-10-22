struct CPSCSpline {
    void* vftable;
};

void __fastcall CPSCSpline_dtor(CPSCSpline* spline);
void operator delete(void*);

void* __fastcall CPSCSpline_scalar_deleting_destructor(CPSCSpline* spline, int edx_unused, unsigned int flags)
{
    CPSCSpline_dtor(spline);
    if (flags & 1)
        operator delete(spline);
    return spline;
}