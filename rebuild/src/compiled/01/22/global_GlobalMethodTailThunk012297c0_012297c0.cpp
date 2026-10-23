struct AutoTinyThunkTarget
{
    void Invoke();
};
extern AutoTinyThunkTarget g_AutoTinyThunkObject_013bac8c;
void __fastcall GlobalMethodTailThunk_012297c0()
{
    g_AutoTinyThunkObject_013bac8c.Invoke();
}