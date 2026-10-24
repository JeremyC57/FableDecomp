struct AutoTinyThunkTarget
// Leaf: CEntry>_const&>
{
    void Invoke();
};
extern AutoTinyThunkTarget g_AutoTinyThunkObject_013bac40;
void __fastcall GlobalMethodTailThunk_01229690()
{
    g_AutoTinyThunkObject_013bac40.Invoke();
}