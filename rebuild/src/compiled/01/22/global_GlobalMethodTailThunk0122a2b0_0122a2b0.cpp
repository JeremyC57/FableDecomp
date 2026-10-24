struct AutoTinyThunkTarget
// Leaf: CEntry>_const&>
{
    void Invoke();
};
extern AutoTinyThunkTarget g_AutoTinyThunkObject_013bb684;
void __fastcall GlobalMethodTailThunk_0122a2b0()
{
    g_AutoTinyThunkObject_013bb684.Invoke();
}