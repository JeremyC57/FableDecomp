struct AutoTinyThunkTarget
{
    void Invoke();
};
extern AutoTinyThunkTarget g_AutoTinyThunkObject_013badf0;
void __fastcall GlobalMethodTailThunk_01229b80()
{
    g_AutoTinyThunkObject_013badf0.Invoke();
}