struct AutoTinyThunkTarget
{
    void Invoke();
};
extern AutoTinyThunkTarget g_AutoTinyThunkObject_013bac98;
void __fastcall GlobalMethodTailThunk_012297f0()
{
    g_AutoTinyThunkObject_013bac98.Invoke();
}