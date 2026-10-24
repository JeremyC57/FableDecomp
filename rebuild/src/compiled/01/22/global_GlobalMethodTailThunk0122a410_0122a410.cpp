struct AutoTinyThunkTarget
// Leaf: CEntry>_const&>
{
    void Invoke();
};
extern AutoTinyThunkTarget g_AutoTinyThunkObject_013bb724;
void __fastcall GlobalMethodTailThunk_0122a410()
{
    g_AutoTinyThunkObject_013bb724.Invoke();
}