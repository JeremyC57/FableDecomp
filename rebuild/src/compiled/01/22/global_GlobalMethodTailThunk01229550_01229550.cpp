struct AutoTinyThunkTarget
// Leaf: CEntry>_const&>
{
    void Invoke();
};
extern AutoTinyThunkTarget g_AutoTinyThunkObject_013babb0;
void __fastcall GlobalMethodTailThunk_01229550()
{
    g_AutoTinyThunkObject_013babb0.Invoke();
}