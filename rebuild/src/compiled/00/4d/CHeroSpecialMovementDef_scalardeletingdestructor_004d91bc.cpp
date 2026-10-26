#pragma optimize("s", on)
class CHeroSpecialMovementDef
{ public:
    void* vftable;
};
extern void* const g_CHeroSpecialMovementDef_vftable;
extern void __fastcall CHeroSpecialMovementDef_Destruct(void* def);
void operator delete(void*);
void* __fastcall scalar_deleting_destructor(CHeroSpecialMovementDef* def, int, unsigned int flags)
{
    def->vftable = (void*)&g_CHeroSpecialMovementDef_vftable;
    CHeroSpecialMovementDef_Destruct(def);
    if (flags & 1)
        operator delete(def);
    return def;
}