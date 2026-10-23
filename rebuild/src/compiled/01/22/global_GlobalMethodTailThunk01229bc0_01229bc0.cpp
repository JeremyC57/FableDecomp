struct AutoTinyThunkTarget
{
    void Invoke();
};
extern AutoTinyThunkTarget g_AutoTinyThunkObject_013bae08;
void __fastcall GlobalMethodTailThunk_01229bc0()
{
    g_AutoTinyThunkObject_013bae08.Invoke();
}