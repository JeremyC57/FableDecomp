struct AutoTinyThunkTarget
{
    void Invoke();
};
extern AutoTinyThunkTarget g_AutoTinyThunkObject_013baf58;
void __fastcall GlobalMethodTailThunk_0122a060()
{
    g_AutoTinyThunkObject_013baf58.Invoke();
}