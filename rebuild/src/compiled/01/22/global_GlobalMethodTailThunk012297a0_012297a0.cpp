struct AutoTinyThunkTarget
// Leaf: CEntry>_const&>
{
    void Invoke();
};
extern AutoTinyThunkTarget g_AutoTinyThunkObject_013bac84;
void __fastcall GlobalMethodTailThunk_012297a0()
{
    g_AutoTinyThunkObject_013bac84.Invoke();
}