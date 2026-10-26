#pragma optimize("s",on)
struct CMember { };
class CTargetingDef { public:
    void* vtbl;
    int Parent;
    void* f8;
    unsigned char pad[0x74 - 0xc];
    CMember member;
    void* CTargetingDef_scalar_deleting_destructor(unsigned char flags);
};

extern void __fastcall CMember_dtor_00451940(CMember* def);
extern void __fastcall CTargetingDef_base_dtor(CTargetingDef* def);
extern void* g_CLandscapeBackgroundPatch_vftable;
extern void __cdecl operator_delete(void* p);

void* CTargetingDef::CTargetingDef_scalar_deleting_destructor(unsigned char flags)
{
    CMember_dtor_00451940(&this->member);
    this->vtbl = &g_CLandscapeBackgroundPatch_vftable;
    CTargetingDef_base_dtor(this);
    if (flags & 1) {
        operator_delete(this);
    }
    return this;
}