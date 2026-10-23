struct AutoTinyThunkTarget
{
    void Invoke();
};
extern AutoTinyThunkTarget g_AutoTinyThunkObject_013b8754;
void __fastcall GlobalMethodTailThunk_01228ad3()
{
    g_AutoTinyThunkObject_013b8754.Invoke();
}