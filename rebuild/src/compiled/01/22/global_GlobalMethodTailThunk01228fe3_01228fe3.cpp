struct AutoTinyThunkTarget
{
    void Invoke();
};
extern AutoTinyThunkTarget g_AutoTinyThunkObject_013b8bac;
void __fastcall GlobalMethodTailThunk_01228fe3()
{
    g_AutoTinyThunkObject_013b8bac.Invoke();
}