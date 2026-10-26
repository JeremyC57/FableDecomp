#pragma optimize("s",on)
class CCombatDialogueDef;
extern void __fastcall CCombatDialogueDef_dtor(CCombatDialogueDef* def);
extern void __cdecl op_delete(void* p);

void* __fastcall CCombatDialogueDef_scalar_deleting_destructor(CCombatDialogueDef* def, int /*edx*/, unsigned int flags)
{
    CCombatDialogueDef_dtor(def);
    if (flags & 1)
        op_delete((void*)def);
    return def;
}