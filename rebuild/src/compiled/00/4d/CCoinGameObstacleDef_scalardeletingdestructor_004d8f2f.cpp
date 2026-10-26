#pragma optimize("s", on)
class CCoinGameObstacleDef
{ public:
    void* vftable;
};
extern void* const g_CCoinGameObstacleDef_vftable;
extern void __fastcall CCoinGameObstacleDef_Destruct(void* def);
void operator delete(void*);
void* __fastcall scalar_deleting_destructor(CCoinGameObstacleDef* def, int, unsigned int flags)
{
    def->vftable = (void*)&g_CCoinGameObstacleDef_vftable;
    CCoinGameObstacleDef_Destruct(def);
    if (flags & 1)
        operator delete(def);
    return def;
}