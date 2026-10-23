struct AutoTinyThunkTarget
{
    void Invoke();
};
extern AutoTinyThunkTarget g_AutoTinyThunkObject_013baf5c;
void __fastcall GlobalMethodTailThunk_0122a070()
{
    g_AutoTinyThunkObject_013baf5c.Invoke();
}