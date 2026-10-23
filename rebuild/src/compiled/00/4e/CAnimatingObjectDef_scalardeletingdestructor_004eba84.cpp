#pragma optimize("s",on)
struct CMember { };
extern void __fastcall CMember_dtor_00461f3b(CMember* def);
extern void __fastcall Base_dtor(void* def);
extern void __cdecl op_delete(void* p);
extern const int g_vtbl;

struct CAnimatingObjectDef {
    int f0;
    int Parent;
    void* f8;
    char pad[0x28 - 0xc];
    CMember m28;
};

void* __fastcall CAnimatingObjectDef_scalar_deleting_destructor(CAnimatingObjectDef* def, int edx, unsigned char flags)
{
    CMember_dtor_00461f3b(&def->m28);
    *(const int**)def = &g_vtbl;
    Base_dtor(def);
    if (flags & 1)
        op_delete(def);
    return def;
}