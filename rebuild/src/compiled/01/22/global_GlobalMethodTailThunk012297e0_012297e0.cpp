struct AutoTinyThunkTarget
{
    void Invoke();
};
extern AutoTinyThunkTarget g_AutoTinyThunkObject_013bac94;
void __fastcall GlobalMethodTailThunk_012297e0()
{
    g_AutoTinyThunkObject_013bac94.Invoke();
}