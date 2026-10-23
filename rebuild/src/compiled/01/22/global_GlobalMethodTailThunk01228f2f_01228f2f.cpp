struct AutoTinyThunkTarget
{
    void Invoke();
};
extern AutoTinyThunkTarget g_AutoTinyThunkObject_013b8b64;
void __fastcall GlobalMethodTailThunk_01228f2f()
{
    g_AutoTinyThunkObject_013b8b64.Invoke();
}