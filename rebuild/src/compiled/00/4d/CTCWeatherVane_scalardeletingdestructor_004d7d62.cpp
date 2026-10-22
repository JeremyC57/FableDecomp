#pragma optimize("s", on)
extern void __fastcall CTCWeatherVane_Destruct(void* vane);
extern void __cdecl OperatorDelete(void* object);
void* __fastcall CTCWeatherVane_ScalarDeletingDestructor(void* vane, int, unsigned int flags)
{
    CTCWeatherVane_Destruct(vane);
    if (flags & 1)
        OperatorDelete(vane);
    return vane;
}