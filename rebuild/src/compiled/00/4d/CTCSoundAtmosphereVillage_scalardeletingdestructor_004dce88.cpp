#pragma optimize("s", on)
extern void __fastcall CTCSoundAtmosphereVillage_Destruct(void* village);
void* __fastcall CTCSoundAtmosphereVillage_ScalarDeletingDestructor(void* village, int, unsigned int flags)
{
    CTCSoundAtmosphereVillage_Destruct(village);
    if (flags & 1)
        operator delete(village);
    return village;
}