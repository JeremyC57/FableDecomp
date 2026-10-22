#pragma optimize("s", on)
extern void __fastcall CVillageDef_Destruct(void* def);
extern void __cdecl OperatorDelete(void* object);
void* __fastcall CVillageDef_ScalarDeletingDestructor(void* def, int, unsigned int flags)
{
    CVillageDef_Destruct(def);
    if (flags & 1)
        OperatorDelete(def);
    return def;
}