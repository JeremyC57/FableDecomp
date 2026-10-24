struct AutoTinyThunkTarget
// Leaf: CEntry>_const&>
{
    void Invoke();
};
extern AutoTinyThunkTarget g_AutoTinyThunkObject_013bac4c;
void __fastcall GlobalMethodTailThunk_012296c0()
{
    g_AutoTinyThunkObject_013bac4c.Invoke();
}