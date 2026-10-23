struct AutoTinyThunkTarget
{
    void Invoke();
};
extern AutoTinyThunkTarget g_AutoTinyThunkObject_013caa4c;
void __fastcall GlobalMethodTailThunk_0122a8e0()
{
    g_AutoTinyThunkObject_013caa4c.Invoke();
}