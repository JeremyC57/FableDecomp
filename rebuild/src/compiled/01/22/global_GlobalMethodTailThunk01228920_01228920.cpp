struct AutoTinyThunkTarget
{
    void Invoke();
};
extern AutoTinyThunkTarget g_AutoTinyThunkObject_013b85e0;
void __fastcall GlobalMethodTailThunk_01228920()
{
    g_AutoTinyThunkObject_013b85e0.Invoke();
}