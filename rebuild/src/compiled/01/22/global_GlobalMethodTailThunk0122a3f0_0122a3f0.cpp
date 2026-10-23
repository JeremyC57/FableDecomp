struct AutoTinyThunkTarget
{
    void Invoke();
};
extern AutoTinyThunkTarget g_AutoTinyThunkObject_013bb71c;
void __fastcall GlobalMethodTailThunk_0122a3f0()
{
    g_AutoTinyThunkObject_013bb71c.Invoke();
}