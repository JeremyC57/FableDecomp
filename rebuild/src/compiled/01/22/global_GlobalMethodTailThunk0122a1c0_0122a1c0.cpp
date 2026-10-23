struct AutoTinyThunkTarget
{
    void Invoke();
};
extern AutoTinyThunkTarget g_AutoTinyThunkObject_013bafcc;
void __fastcall GlobalMethodTailThunk_0122a1c0()
{
    g_AutoTinyThunkObject_013bafcc.Invoke();
}