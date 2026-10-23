struct AutoTinyThunkTarget
{
    void Invoke();
};
extern AutoTinyThunkTarget g_AutoTinyThunkObject_013bb708;
void __fastcall GlobalMethodTailThunk_0122a3e0()
{
    g_AutoTinyThunkObject_013bb708.Invoke();
}