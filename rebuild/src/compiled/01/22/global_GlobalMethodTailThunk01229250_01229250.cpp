struct AutoTinyThunkTarget
// Leaf: CEntry>_const&>
{
    void Invoke();
};
extern AutoTinyThunkTarget g_AutoTinyThunkObject_013ba898;
void __fastcall GlobalMethodTailThunk_01229250()
{
    g_AutoTinyThunkObject_013ba898.Invoke();
}