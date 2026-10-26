#pragma optimize("s", on)
extern void __fastcall CTCPlot_Destruct(void* plot);
void* __fastcall CTCPlot_ScalarDeletingDestructor(void* plot, int, unsigned int flags)
{
    CTCPlot_Destruct(plot);
    if (flags & 1)
        operator delete(plot);
    return plot;
}