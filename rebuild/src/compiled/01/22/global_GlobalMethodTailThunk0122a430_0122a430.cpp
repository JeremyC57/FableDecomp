struct AutoTinyThunkTarget
{
    void Invoke();
};
extern AutoTinyThunkTarget g_AutoTinyThunkObject_013bb72c;
void __fastcall GlobalMethodTailThunk_0122a430()
{
    g_AutoTinyThunkObject_013bb72c.Invoke();
}