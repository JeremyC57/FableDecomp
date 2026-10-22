#pragma optimize("s",on)
struct CSpecialAbilitiesBerserkDef { int vt; };

extern void __fastcall CSpecialAbilitiesBerserkDef_dtor(CSpecialAbilitiesBerserkDef* def);
extern void __cdecl op_delete(void* p);

void* __fastcall CSpecialAbilitiesBerserkDef_scalar_deleting_destructor(CSpecialAbilitiesBerserkDef* def, int edx, unsigned int flags)
{
    CSpecialAbilitiesBerserkDef_dtor(def);
    if (flags & 1)
        op_delete(def);
    return def;
}