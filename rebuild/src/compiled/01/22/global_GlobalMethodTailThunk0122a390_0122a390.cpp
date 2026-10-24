struct AutoTinyThunkTarget
// Leaf: CEntry>_const&>
{
    void Invoke();
};
extern AutoTinyThunkTarget g_AutoTinyThunkObject_013bb6e4;
void __fastcall GlobalMethodTailThunk_0122a390()
{
    g_AutoTinyThunkObject_013bb6e4.Invoke();
}