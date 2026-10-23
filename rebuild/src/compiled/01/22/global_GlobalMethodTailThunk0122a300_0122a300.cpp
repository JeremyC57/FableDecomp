struct AutoTinyThunkTarget
{
    void Invoke();
};
extern AutoTinyThunkTarget g_AutoTinyThunkObject_013bb6c0;
void __fastcall GlobalMethodTailThunk_0122a300()
{
    g_AutoTinyThunkObject_013bb6c0.Invoke();
}