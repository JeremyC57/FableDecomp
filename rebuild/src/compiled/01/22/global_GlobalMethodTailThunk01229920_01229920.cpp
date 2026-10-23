struct AutoTinyThunkTarget
{
    void Invoke();
};
extern AutoTinyThunkTarget g_AutoTinyThunkObject_013bace4;
void __fastcall GlobalMethodTailThunk_01229920()
{
    g_AutoTinyThunkObject_013bace4.Invoke();
}