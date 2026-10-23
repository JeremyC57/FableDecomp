struct AutoTinyThunkTarget
{
    void Invoke();
};
extern AutoTinyThunkTarget g_AutoTinyThunkObject_013bac74;
void __fastcall GlobalMethodTailThunk_01229760()
{
    g_AutoTinyThunkObject_013bac74.Invoke();
}