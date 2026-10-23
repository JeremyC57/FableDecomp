struct AutoTinyThunkTarget
{
    void Invoke();
};
extern AutoTinyThunkTarget g_AutoTinyThunkObject_013b8bbc;
void __fastcall GlobalMethodTailThunk_0122900b()
{
    g_AutoTinyThunkObject_013b8bbc.Invoke();
}