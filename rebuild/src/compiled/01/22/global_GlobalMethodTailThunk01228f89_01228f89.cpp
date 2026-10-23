struct AutoTinyThunkTarget
{
    void Invoke();
};
extern AutoTinyThunkTarget g_AutoTinyThunkObject_013b8b88;
void __fastcall GlobalMethodTailThunk_01228f89()
{
    g_AutoTinyThunkObject_013b8b88.Invoke();
}