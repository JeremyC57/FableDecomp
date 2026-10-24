struct AutoTinyThunkTarget
// Leaf: CEntry>_const&>
{
    void Invoke();
};
extern AutoTinyThunkTarget g_AutoTinyThunkObject_013bb668;
void __fastcall GlobalMethodTailThunk_0122a240()
{
    g_AutoTinyThunkObject_013bb668.Invoke();
}