struct AutoTinyThunkTarget
// Leaf: CEntry>_const&>
{
    void Invoke();
};
extern AutoTinyThunkTarget g_AutoTinyThunkObject_013bafa0;
void __fastcall GlobalMethodTailThunk_0122a080()
{
    g_AutoTinyThunkObject_013bafa0.Invoke();
}