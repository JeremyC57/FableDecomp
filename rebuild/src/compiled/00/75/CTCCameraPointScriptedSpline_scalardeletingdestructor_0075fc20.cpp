struct CTCCameraPointScriptedSpline {
    void* vftable;
};

void __fastcall CTCCameraPointScriptedSpline_dtor(CTCCameraPointScriptedSpline* spline);
void operator delete(void*);

void* __fastcall CTCCameraPointScriptedSpline_scalar_deleting_destructor(CTCCameraPointScriptedSpline* spline, int edx_unused, unsigned int flags)
{
    CTCCameraPointScriptedSpline_dtor(spline);
    if (flags & 1)
        operator delete(spline);
    return spline;
}