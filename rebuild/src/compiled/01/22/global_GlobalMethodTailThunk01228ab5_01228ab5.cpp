struct AutoTinyThunkTarget
{
    void Invoke();
};
extern AutoTinyThunkTarget g_AutoTinyThunkObject_013b8748;
void __fastcall GlobalMethodTailThunk_01228ab5()
{
    g_AutoTinyThunkObject_013b8748.Invoke();
}