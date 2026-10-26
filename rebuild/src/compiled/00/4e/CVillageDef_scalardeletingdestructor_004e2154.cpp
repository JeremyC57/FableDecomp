#pragma optimize("s", on)
extern void __fastcall CVillageDef_Destruct(void* def);
void* __fastcall CVillageDef_ScalarDeletingDestructor(void* def, int, unsigned int flags)
{
    CVillageDef_Destruct(def);
    if (flags & 1)
        operator delete(def);
    return def;
}