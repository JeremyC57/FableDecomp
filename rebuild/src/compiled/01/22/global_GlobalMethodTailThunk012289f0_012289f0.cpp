struct AutoTinyThunkTarget
{
    void Invoke();
};
extern AutoTinyThunkTarget g_AutoTinyThunkObject_013b86f4;
void __fastcall GlobalMethodTailThunk_012289f0()
{
    g_AutoTinyThunkObject_013b86f4.Invoke();
}