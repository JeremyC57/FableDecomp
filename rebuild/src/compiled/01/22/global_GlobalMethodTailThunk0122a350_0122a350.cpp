struct AutoTinyThunkTarget
{
    void Invoke();
};
extern AutoTinyThunkTarget g_AutoTinyThunkObject_013bb6d4;
void __fastcall GlobalMethodTailThunk_0122a350()
{
    g_AutoTinyThunkObject_013bb6d4.Invoke();
}