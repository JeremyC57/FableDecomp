#pragma optimize("s", on)
class CPrizeScoreDef
{ public:
    void* vftable;
};
extern void* const g_CPrizeScoreDef_vftable;
extern void __fastcall CPrizeScoreDef_Destruct(void* def);
void operator delete(void*);
void* __fastcall scalar_deleting_destructor(CPrizeScoreDef* def, int, unsigned int flags)
{
    def->vftable = (void*)&g_CPrizeScoreDef_vftable;
    CPrizeScoreDef_Destruct(def);
    if (flags & 1)
        operator delete(def);
    return def;
}