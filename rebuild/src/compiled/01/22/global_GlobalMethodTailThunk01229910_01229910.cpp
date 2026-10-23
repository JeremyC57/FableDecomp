struct AutoTinyThunkTarget
{
    void Invoke();
};
extern AutoTinyThunkTarget g_AutoTinyThunkObject_013bace0;
void __fastcall GlobalMethodTailThunk_01229910()
{
    g_AutoTinyThunkObject_013bace0.Invoke();
}