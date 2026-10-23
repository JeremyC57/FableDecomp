struct AutoTinyThunkTarget
{
    void Invoke();
};
extern AutoTinyThunkTarget g_AutoTinyThunkObject_013bac3c;
void __fastcall GlobalMethodTailThunk_01229680()
{
    g_AutoTinyThunkObject_013bac3c.Invoke();
}