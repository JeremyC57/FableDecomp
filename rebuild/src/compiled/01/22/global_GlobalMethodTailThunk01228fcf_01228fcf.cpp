struct AutoTinyThunkTarget
{
    void Invoke();
};
extern AutoTinyThunkTarget g_AutoTinyThunkObject_013b8ba4;
void __fastcall GlobalMethodTailThunk_01228fcf()
{
    g_AutoTinyThunkObject_013b8ba4.Invoke();
}