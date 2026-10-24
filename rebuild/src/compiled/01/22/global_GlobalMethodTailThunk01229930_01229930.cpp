struct AutoTinyThunkTarget
// Leaf: CEntry>_const&>
{
    void Invoke();
};
extern AutoTinyThunkTarget g_AutoTinyThunkObject_013bace8;
void __fastcall GlobalMethodTailThunk_01229930()
{
    g_AutoTinyThunkObject_013bace8.Invoke();
}