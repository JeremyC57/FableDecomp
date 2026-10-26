#pragma optimize("s",on)
// CGameDefinitionManager::`scalar deleting destructor'
// this in ecx (__fastcall), flags on stack. Calls scalar dtor then optional delete.
extern void __fastcall CGameDefinitionManager_dtor(void* manager);

void* __fastcall CGameDefinitionManager_scalar_deleting_destructor(void* manager, int /*edx*/, unsigned int flags)
{
    CGameDefinitionManager_dtor(manager);
    if (flags & 1)
        operator delete(manager);
    return manager;
}