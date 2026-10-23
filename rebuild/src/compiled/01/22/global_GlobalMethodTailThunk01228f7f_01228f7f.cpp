struct AutoTinyThunkTarget
{
    void Invoke();
};
extern AutoTinyThunkTarget g_AutoTinyThunkObject_013b8b84;
void __fastcall GlobalMethodTailThunk_01228f7f()
{
    g_AutoTinyThunkObject_013b8b84.Invoke();
}