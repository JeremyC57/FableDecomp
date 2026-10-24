struct AutoTinyThunkTarget
// Leaf: CEntry>_const&>
{
    void Invoke();
};
extern AutoTinyThunkTarget g_AutoTinyThunkObject_013bab08;
void __fastcall GlobalMethodTailThunk_012293e0()
{
    g_AutoTinyThunkObject_013bab08.Invoke();
}