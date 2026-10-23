struct AutoTinyThunkTarget
{
    void Invoke();
};
extern AutoTinyThunkTarget g_AutoTinyThunkObject_013bacdc;
void __fastcall GlobalMethodTailThunk_01229900()
{
    g_AutoTinyThunkObject_013bacdc.Invoke();
}