struct AutoTinyThunkTarget
{
    void Invoke();
};
extern AutoTinyThunkTarget g_AutoTinyThunkObject_013ba878;
void __fastcall GlobalMethodTailThunk_012291d0()
{
    g_AutoTinyThunkObject_013ba878.Invoke();
}