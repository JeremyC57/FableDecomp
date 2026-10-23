struct AutoTinyThunkTarget
{
    void Invoke();
};
extern AutoTinyThunkTarget g_AutoTinyThunkObject_013d27e4;
void __fastcall GlobalMethodTailThunk_0122aa60()
{
    g_AutoTinyThunkObject_013d27e4.Invoke();
}