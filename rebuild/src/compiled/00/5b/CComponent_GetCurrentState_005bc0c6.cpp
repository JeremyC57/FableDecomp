struct CUIState;

extern CUIState* __stdcall NUISystem_GetState_005bbf80(char* p);

CUIState* __fastcall CComponent_GetCurrentState(void* self)
{
    char local;
    return NUISystem_GetState_005bbf80(&local);
}