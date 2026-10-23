struct AutoTinyThunkTarget
{
    void Invoke();
};
extern AutoTinyThunkTarget g_AutoTinyThunkObject_013b8b54;
void __fastcall GlobalMethodTailThunk_01228f00()
{
    g_AutoTinyThunkObject_013b8b54.Invoke();
}