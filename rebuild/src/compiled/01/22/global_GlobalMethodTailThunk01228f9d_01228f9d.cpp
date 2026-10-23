struct AutoTinyThunkTarget
{
    void Invoke();
};
extern AutoTinyThunkTarget g_AutoTinyThunkObject_013b8b90;
void __fastcall GlobalMethodTailThunk_01228f9d()
{
    g_AutoTinyThunkObject_013b8b90.Invoke();
}