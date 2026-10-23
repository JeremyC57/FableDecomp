struct AutoTinyThunkTarget
{
    void Invoke();
};
extern AutoTinyThunkTarget g_AutoTinyThunkObject_013bac5c;
void __fastcall GlobalMethodTailThunk_01229700()
{
    g_AutoTinyThunkObject_013bac5c.Invoke();
}