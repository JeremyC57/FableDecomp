struct AutoTinyThunkTarget
{
    void Invoke();
};
extern AutoTinyThunkTarget g_AutoTinyThunkObject_013b86f8;
void __fastcall GlobalMethodTailThunk_01228a00()
{
    g_AutoTinyThunkObject_013b86f8.Invoke();
}