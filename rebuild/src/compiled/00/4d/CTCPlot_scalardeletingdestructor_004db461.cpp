#pragma optimize("s", on)
extern void __fastcall CTCPlot_Destruct(void* plot);
extern void __cdecl OperatorDelete(void* object);
void* __fastcall CTCPlot_ScalarDeletingDestructor(void* plot, int, unsigned int flags)
{
    CTCPlot_Destruct(plot);
    if (flags & 1)
        OperatorDelete(plot);
    return plot;
}