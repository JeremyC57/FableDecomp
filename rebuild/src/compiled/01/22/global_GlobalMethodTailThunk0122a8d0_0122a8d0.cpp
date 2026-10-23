struct AutoTinyThunkTarget
{
    void Invoke();
};
extern AutoTinyThunkTarget g_AutoTinyThunkObject_013caa48;
void __fastcall GlobalMethodTailThunk_0122a8d0()
{
    g_AutoTinyThunkObject_013caa48.Invoke();
}