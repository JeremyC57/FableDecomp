#pragma optimize("s", on)
class CFireballSpellLevelDef
{ public:
    void* vftable;
};
extern void* const g_CFireballSpellLevelDef_vftable;
extern void __fastcall CFireballSpellLevelDef_Destruct(void* def);
void operator delete(void*);
void* __fastcall scalar_deleting_destructor(CFireballSpellLevelDef* def, int, unsigned int flags)
{
    def->vftable = (void*)&g_CFireballSpellLevelDef_vftable;
    CFireballSpellLevelDef_Destruct(def);
    if (flags & 1)
        operator delete(def);
    return def;
}