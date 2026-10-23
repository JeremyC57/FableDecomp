struct AutoTinyThunkTarget
{
    void Invoke();
};
extern AutoTinyThunkTarget g_AutoTinyThunkObject_013bac1c;
void __fastcall GlobalMethodTailThunk_01229620()
{
    g_AutoTinyThunkObject_013bac1c.Invoke();
}