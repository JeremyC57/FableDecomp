struct AutoTinyThunkTarget
{
    void Invoke();
};
extern AutoTinyThunkTarget g_AutoTinyThunkObject_013caa50;
void __fastcall GlobalMethodTailThunk_0122a8f0()
{
    g_AutoTinyThunkObject_013caa50.Invoke();
}