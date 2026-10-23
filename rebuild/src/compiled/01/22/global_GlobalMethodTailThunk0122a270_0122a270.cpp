struct AutoTinyThunkTarget
{
    void Invoke();
};
extern AutoTinyThunkTarget g_AutoTinyThunkObject_013bb674;
void __fastcall GlobalMethodTailThunk_0122a270()
{
    g_AutoTinyThunkObject_013bb674.Invoke();
}