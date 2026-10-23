struct AutoTinyThunkTarget
{
    void Invoke();
};
extern AutoTinyThunkTarget g_AutoTinyThunkObject_013bb66c;
void __fastcall GlobalMethodTailThunk_0122a250()
{
    g_AutoTinyThunkObject_013bb66c.Invoke();
}