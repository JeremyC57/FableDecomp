struct AutoTinyThunkTarget
// Leaf: CEntry>_const&>
{
    void Invoke();
};
extern AutoTinyThunkTarget g_AutoTinyThunkObject_013bb6ec;
void __fastcall GlobalMethodTailThunk_0122a3b0()
{
    g_AutoTinyThunkObject_013bb6ec.Invoke();
}