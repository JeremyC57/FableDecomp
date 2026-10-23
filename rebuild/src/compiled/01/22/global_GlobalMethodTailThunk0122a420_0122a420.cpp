struct AutoTinyThunkTarget
{
    void Invoke();
};
extern AutoTinyThunkTarget g_AutoTinyThunkObject_013bb728;
void __fastcall GlobalMethodTailThunk_0122a420()
{
    g_AutoTinyThunkObject_013bb728.Invoke();
}