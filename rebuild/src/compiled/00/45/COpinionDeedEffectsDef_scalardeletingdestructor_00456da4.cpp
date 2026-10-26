#pragma optimize("s",on)
class COpinionDeedEffectsDef { public: int f0; };
extern "C" void __fastcall COpinionDeedEffectsDef_dtor(COpinionDeedEffectsDef* def);
extern "C" void operator_delete_impl(void* p);

void* __fastcall COpinionDeedEffectsDef_scalar_deleting_destructor(COpinionDeedEffectsDef* def, int /*edx*/, unsigned int flags)
{
    COpinionDeedEffectsDef_dtor(def);
    if (flags & 1)
        operator_delete_impl(def);
    return def;
}