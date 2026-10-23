struct AutoTinyThunkTarget
{
    void Invoke();
};
extern AutoTinyThunkTarget g_AutoTinyThunkObject_013bab1c;
void __fastcall GlobalMethodTailThunk_01229450()
{
    g_AutoTinyThunkObject_013bab1c.Invoke();
}