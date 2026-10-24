struct AutoTinyThunkTarget
// Leaf: CEntry>_const&>
{
    void Invoke();
};
extern AutoTinyThunkTarget g_AutoTinyThunkObject_013bac70;
void __fastcall GlobalMethodTailThunk_01229750()
{
    g_AutoTinyThunkObject_013bac70.Invoke();
}