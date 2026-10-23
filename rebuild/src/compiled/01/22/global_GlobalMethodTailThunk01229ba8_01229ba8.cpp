struct AutoTinyThunkTarget
{
    void Invoke();
};
extern AutoTinyThunkTarget g_AutoTinyThunkObject_013bae00;
void __fastcall GlobalMethodTailThunk_01229ba8()
{
    g_AutoTinyThunkObject_013bae00.Invoke();
}