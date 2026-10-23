struct AutoTinyThunkTarget
{
    void Invoke();
};
extern AutoTinyThunkTarget g_AutoTinyThunkObject_013b8b50;
void __fastcall GlobalMethodTailThunk_01228ef6()
{
    g_AutoTinyThunkObject_013b8b50.Invoke();
}