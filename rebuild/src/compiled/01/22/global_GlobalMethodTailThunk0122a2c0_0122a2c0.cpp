struct AutoTinyThunkTarget
{
    void Invoke();
};
extern AutoTinyThunkTarget g_AutoTinyThunkObject_013bb688;
void __fastcall GlobalMethodTailThunk_0122a2c0()
{
    g_AutoTinyThunkObject_013bb688.Invoke();
}