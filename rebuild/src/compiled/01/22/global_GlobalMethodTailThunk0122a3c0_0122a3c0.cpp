struct AutoTinyThunkTarget
{
    void Invoke();
};
extern AutoTinyThunkTarget g_AutoTinyThunkObject_013bb6f0;
void __fastcall GlobalMethodTailThunk_0122a3c0()
{
    g_AutoTinyThunkObject_013bb6f0.Invoke();
}