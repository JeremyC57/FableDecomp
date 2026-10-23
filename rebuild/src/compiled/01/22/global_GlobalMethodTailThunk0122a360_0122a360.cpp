struct AutoTinyThunkTarget
{
    void Invoke();
};
extern AutoTinyThunkTarget g_AutoTinyThunkObject_013bb6d8;
void __fastcall GlobalMethodTailThunk_0122a360()
{
    g_AutoTinyThunkObject_013bb6d8.Invoke();
}