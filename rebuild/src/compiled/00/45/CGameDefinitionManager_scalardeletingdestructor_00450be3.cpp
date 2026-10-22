#pragma optimize("s",on)
// CGameDefinitionManager::`scalar deleting destructor'
// this in ecx (__fastcall), flags on stack. Calls scalar dtor then optional delete.
extern void __fastcall CGameDefinitionManager_dtor(void* manager);
extern void __cdecl operator_delete_impl(void* p);

void* __fastcall CGameDefinitionManager_scalar_deleting_destructor(void* manager, int /*edx*/, unsigned int flags)
{
    CGameDefinitionManager_dtor(manager);
    if (flags & 1)
        operator_delete_impl(manager);
    return manager;
}