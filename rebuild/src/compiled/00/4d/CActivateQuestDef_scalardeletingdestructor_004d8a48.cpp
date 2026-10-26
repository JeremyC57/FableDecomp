#pragma optimize("s", on)
class CActivateQuestDef
{ public:
    void* vftable;
};
extern void* const g_CActivateQuestDef_vftable;
extern void __fastcall CActivateQuestDef_Destruct(void* def);
void operator delete(void*);
void* __fastcall scalar_deleting_destructor(CActivateQuestDef* def, int, unsigned int flags)
{
    def->vftable = (void*)&g_CActivateQuestDef_vftable;
    CActivateQuestDef_Destruct(def);
    if (flags & 1)
        operator delete(def);
    return def;
}