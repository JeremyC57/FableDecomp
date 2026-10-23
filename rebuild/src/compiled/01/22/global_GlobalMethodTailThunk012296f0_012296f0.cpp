struct AutoTinyThunkTarget
{
    void Invoke();
};
extern AutoTinyThunkTarget g_AutoTinyThunkObject_013bac58;
void __fastcall GlobalMethodTailThunk_012296f0()
{
    g_AutoTinyThunkObject_013bac58.Invoke();
}