struct AutoTinyThunkTarget
{
    void Invoke();
};
extern AutoTinyThunkTarget g_AutoTinyThunkObject_013bb720;
void __fastcall GlobalMethodTailThunk_0122a400()
{
    g_AutoTinyThunkObject_013bb720.Invoke();
}