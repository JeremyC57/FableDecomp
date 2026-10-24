struct AutoTinyThunkTarget
// Leaf: CEntry>_const&>
{
    void Invoke();
};
extern AutoTinyThunkTarget g_AutoTinyThunkObject_013b875c;
void __fastcall GlobalMethodTailThunk_01228ae7()
{
    g_AutoTinyThunkObject_013b875c.Invoke();
}