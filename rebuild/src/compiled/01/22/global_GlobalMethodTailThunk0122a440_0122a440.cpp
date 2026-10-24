struct AutoTinyThunkTarget
// Leaf: CEntry>_const&>
{
    void Invoke();
};
extern AutoTinyThunkTarget g_AutoTinyThunkObject_013bb734;
void __fastcall GlobalMethodTailThunk_0122a440()
{
    g_AutoTinyThunkObject_013bb734.Invoke();
}