struct AutoTinyThunkTarget
{
    void Invoke();
};
extern AutoTinyThunkTarget g_AutoTinyThunkObject_013b8b58;
void __fastcall GlobalMethodTailThunk_01228f0a()
{
    g_AutoTinyThunkObject_013b8b58.Invoke();
}