struct AutoTinyThunkTarget
{
    void Invoke();
};
extern AutoTinyThunkTarget g_AutoTinyThunkObject_013bab00;
void __fastcall GlobalMethodTailThunk_012293c0()
{
    g_AutoTinyThunkObject_013bab00.Invoke();
}