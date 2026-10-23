struct AutoTinyThunkTarget
{
    void Invoke();
};
extern AutoTinyThunkTarget g_AutoTinyThunkObject_013b8700;
void __fastcall GlobalMethodTailThunk_01228a20()
{
    g_AutoTinyThunkObject_013b8700.Invoke();
}