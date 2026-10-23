struct AutoTinyThunkTarget
{
    void Invoke();
};
extern AutoTinyThunkTarget g_AutoTinyThunkObject_013bb6c4;
void __fastcall GlobalMethodTailThunk_0122a310()
{
    g_AutoTinyThunkObject_013bb6c4.Invoke();
}