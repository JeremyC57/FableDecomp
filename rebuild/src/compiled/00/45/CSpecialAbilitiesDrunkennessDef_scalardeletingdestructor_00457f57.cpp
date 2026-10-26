#pragma optimize("s",on)
// MSVC vector deleting destructor for CSpecialAbilitiesDrunkennessDef
class CSpecialAbilitiesDrunkennessDef { public:
    void* vftable;
};

extern "C" void __fastcall CSpecialAbilitiesDrunkennessDef_dtor(CSpecialAbilitiesDrunkennessDef* def);
extern "C" void __cdecl operator_delete_stub(void* p);

void* __fastcall CSpecialAbilitiesDrunkennessDef_scalar_deleting_destructor(CSpecialAbilitiesDrunkennessDef* def, int /*edx*/, unsigned int flags)
{
    CSpecialAbilitiesDrunkennessDef_dtor(def);
    if (flags & 1)
        operator_delete_stub(def);
    return def;
}