struct AutoTinyThunkTarget
// Leaf: CEntry>_const&>
{
    void Invoke();
};
extern AutoTinyThunkTarget g_AutoTinyThunkObject_013bac90;
void __fastcall GlobalMethodTailThunk_012297d0()
{
    g_AutoTinyThunkObject_013bac90.Invoke();
}