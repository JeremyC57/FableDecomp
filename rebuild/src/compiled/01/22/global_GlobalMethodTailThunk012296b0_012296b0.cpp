struct AutoTinyThunkTarget
{
    void Invoke();
};
extern AutoTinyThunkTarget g_AutoTinyThunkObject_013bac48;
void __fastcall GlobalMethodTailThunk_012296b0()
{
    g_AutoTinyThunkObject_013bac48.Invoke();
}