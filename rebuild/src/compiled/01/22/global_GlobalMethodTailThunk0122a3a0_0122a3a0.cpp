struct AutoTinyThunkTarget
// Leaf: CEntry>_const&>
{
    void Invoke();
};
extern AutoTinyThunkTarget g_AutoTinyThunkObject_013bb6e8;
void __fastcall GlobalMethodTailThunk_0122a3a0()
{
    g_AutoTinyThunkObject_013bb6e8.Invoke();
}