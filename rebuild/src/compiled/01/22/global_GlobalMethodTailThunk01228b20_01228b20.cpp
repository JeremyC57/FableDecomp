struct AutoTinyThunkTarget
{
    void Invoke();
};
extern AutoTinyThunkTarget g_AutoTinyThunkObject_013b8788;
void __fastcall GlobalMethodTailThunk_01228b20()
{
    g_AutoTinyThunkObject_013b8788.Invoke();
}