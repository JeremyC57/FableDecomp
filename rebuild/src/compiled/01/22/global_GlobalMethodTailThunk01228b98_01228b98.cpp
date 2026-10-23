struct AutoTinyThunkTarget
{
    void Invoke();
};
extern AutoTinyThunkTarget g_AutoTinyThunkObject_013b8798;
void __fastcall GlobalMethodTailThunk_01228b98()
{
    g_AutoTinyThunkObject_013b8798.Invoke();
}