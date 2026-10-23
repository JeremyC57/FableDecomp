struct AutoTinyThunkTarget
{
    void Invoke();
};
extern AutoTinyThunkTarget g_AutoTinyThunkObject_013ba87c;
void __fastcall GlobalMethodTailThunk_012291e0()
{
    g_AutoTinyThunkObject_013ba87c.Invoke();
}