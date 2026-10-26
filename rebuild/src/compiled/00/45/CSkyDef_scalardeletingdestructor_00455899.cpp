#pragma optimize("s",on)
class CSkyDef;
extern void __fastcall Sub_Dtor_00453327(void* p);
extern void __fastcall CSkyDef_Dtor(CSkyDef* def);
extern void __cdecl operator_delete(void* p);

void* __fastcall CSkyDef_scalar_deleting_destructor(CSkyDef* def, int edx, unsigned int flags)
{
    Sub_Dtor_00453327((char*)def + 0x74);
    CSkyDef_Dtor(def);
    if (flags & 1)
        operator_delete(def);
    return def;
}