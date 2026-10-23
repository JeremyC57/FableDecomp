struct AutoTinyThunkTarget
{
    void Invoke();
};
extern AutoTinyThunkTarget g_AutoTinyThunkObject_013bafd4;
void __fastcall GlobalMethodTailThunk_0122a1d0()
{
    g_AutoTinyThunkObject_013bafd4.Invoke();
}