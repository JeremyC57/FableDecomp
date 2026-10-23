struct AutoTinyThunkTarget
{
    void Invoke();
};
extern AutoTinyThunkTarget g_AutoTinyThunkObject_013b8b94;
void __fastcall GlobalMethodTailThunk_01228fa7()
{
    g_AutoTinyThunkObject_013b8b94.Invoke();
}