struct AutoTinyThunkTarget
{
    void Invoke();
};
extern AutoTinyThunkTarget g_AutoTinyThunkObject_013bac78;
void __fastcall GlobalMethodTailThunk_01229770()
{
    g_AutoTinyThunkObject_013bac78.Invoke();
}