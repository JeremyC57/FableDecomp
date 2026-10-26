#pragma optimize("s",on)
class CInventoryCategoryDef { public: int f0; };
extern void __fastcall CInventoryCategoryDef_dtor(CInventoryCategoryDef* def);
extern void __cdecl operator_delete_impl(void* p);

void* __fastcall CInventoryCategoryDef_scalar_deleting_destructor(CInventoryCategoryDef* def, int /*edx*/, unsigned int flags)
{
    CInventoryCategoryDef_dtor(def);
    if (flags & 1)
        operator_delete_impl(def);
    return def;
}