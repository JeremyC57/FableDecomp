struct AutoTinyThunkTarget
// Leaf: CEntry>_const&>
{
    void Invoke();
};
extern AutoTinyThunkTarget g_AutoTinyThunkObject_013baf24;
void __fastcall GlobalMethodTailThunk_01229f10()
{
    g_AutoTinyThunkObject_013baf24.Invoke();
}