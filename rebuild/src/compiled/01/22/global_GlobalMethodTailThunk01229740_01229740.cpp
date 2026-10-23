struct AutoTinyThunkTarget
{
    void Invoke();
};
extern AutoTinyThunkTarget g_AutoTinyThunkObject_013bac6c;
void __fastcall GlobalMethodTailThunk_01229740()
{
    g_AutoTinyThunkObject_013bac6c.Invoke();
}