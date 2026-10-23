struct AutoTinyThunkTarget
{
    void Invoke();
};
extern AutoTinyThunkTarget g_AutoTinyThunkObject_013bb690;
void __fastcall GlobalMethodTailThunk_0122a2d0()
{
    g_AutoTinyThunkObject_013bb690.Invoke();
}