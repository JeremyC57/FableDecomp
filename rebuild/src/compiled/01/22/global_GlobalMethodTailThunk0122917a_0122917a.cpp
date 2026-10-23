struct AutoTinyThunkTarget
{
    void Invoke();
};
extern AutoTinyThunkTarget g_AutoTinyThunkObject_013b8c60;
void __fastcall GlobalMethodTailThunk_0122917a()
{
    g_AutoTinyThunkObject_013b8c60.Invoke();
}