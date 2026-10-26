#pragma optimize("s",on)
// scalar deleting destructor for CSoundThemeDef
class CSoundThemeDef { public: char pad[0x58]; };

extern "C" void __fastcall Sub54_dtor(void* ecx);
extern "C" void __fastcall CSoundThemeDef_realdtor(void* ecx);
void operator delete(void* p);

void* __fastcall CSoundThemeDef_scalar_deleting_destructor(CSoundThemeDef* def, int edx, unsigned int flags)
{
    (void)edx;
    Sub54_dtor((char*)def + 0x54);
    CSoundThemeDef_realdtor(def);
    if (flags & 1)
        operator delete(def);
    return def;
}