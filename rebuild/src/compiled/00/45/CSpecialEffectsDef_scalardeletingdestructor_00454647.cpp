#pragma optimize("s",on)
struct CMember { };
extern void __fastcall CMember_dtor_004518e1(CMember* def);
extern void __fastcall Base_dtor(void* def);
extern void __cdecl op_delete(void* p);
extern const int g_vtbl;

class CSpecialEffectsDef { public:
    int f0;
    int Parent;
    void* f8;
    char pad[0x28 - 0xc];
    CMember m28;
};

void* __fastcall CSpecialEffectsDef_scalar_deleting_destructor(CSpecialEffectsDef* def, int edx, unsigned char flags)
{
    CMember_dtor_004518e1(&def->m28);
    *(const int**)def = &g_vtbl;
    Base_dtor(def);
    if (flags & 1)
        op_delete(def);
    return def;
}