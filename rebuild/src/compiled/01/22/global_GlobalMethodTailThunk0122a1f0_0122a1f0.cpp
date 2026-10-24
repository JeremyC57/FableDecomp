struct AutoTinyThunkTarget
// Leaf: CEntry>_const&>
{
    void Invoke();
};
extern AutoTinyThunkTarget g_AutoTinyThunkObject_013baff8;
void __fastcall GlobalMethodTailThunk_0122a1f0()
{
    g_AutoTinyThunkObject_013baff8.Invoke();
}