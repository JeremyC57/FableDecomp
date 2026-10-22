#pragma optimize("s",on)
struct CBase;
struct CManager {
    void* pad0;
    CBase** screens;
    char pad8[0x28 - 8];
    int current;
};
CBase* __fastcall CManager_GetCurrentScreen(CManager* manager)
{
    CBase** arr = manager->screens;
    if (arr[manager->current] == 0)
        manager->current = 0;
    return arr[manager->current];
}