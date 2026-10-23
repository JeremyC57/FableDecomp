struct AutoTinyThunkTarget
{
    void Invoke();
};
extern AutoTinyThunkTarget g_AutoTinyThunkObject_013baafc;
void __fastcall GlobalMethodTailThunk_012293b0()
{
    g_AutoTinyThunkObject_013baafc.Invoke();
}