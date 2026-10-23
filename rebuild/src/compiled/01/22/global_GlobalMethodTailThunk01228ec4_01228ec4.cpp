struct AutoTinyThunkTarget
{
    void Invoke();
};
extern AutoTinyThunkTarget g_AutoTinyThunkObject_013b8b38;
void __fastcall GlobalMethodTailThunk_01228ec4()
{
    g_AutoTinyThunkObject_013b8b38.Invoke();
}