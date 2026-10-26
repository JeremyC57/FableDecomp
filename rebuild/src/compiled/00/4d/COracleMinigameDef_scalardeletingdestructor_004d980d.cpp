#pragma optimize("s", on)
class COracleMinigameDef
{ public:
    void* vftable;
};
extern void* const g_COracleMinigameDef_vftable;
extern void __fastcall COracleMinigameDef_Destruct(void* def);
void operator delete(void*);
void* __fastcall scalar_deleting_destructor(COracleMinigameDef* def, int, unsigned int flags)
{
    def->vftable = (void*)&g_COracleMinigameDef_vftable;
    COracleMinigameDef_Destruct(def);
    if (flags & 1)
        operator delete(def);
    return def;
}