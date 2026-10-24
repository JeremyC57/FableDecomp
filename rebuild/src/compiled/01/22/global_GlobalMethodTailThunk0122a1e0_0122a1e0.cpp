struct AutoTinyThunkTarget
// Leaf: CEntry>_const&>
{
    void Invoke();
};
extern AutoTinyThunkTarget g_AutoTinyThunkObject_013baff4;
void __fastcall GlobalMethodTailThunk_0122a1e0()
{
    g_AutoTinyThunkObject_013baff4.Invoke();
}