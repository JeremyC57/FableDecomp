struct AutoTinyThunkTarget
// Leaf: CEntry>_const&>
{
    void Invoke();
};
extern AutoTinyThunkTarget g_AutoTinyThunkObject_013bb6d0;
void __fastcall GlobalMethodTailThunk_0122a340()
{
    g_AutoTinyThunkObject_013bb6d0.Invoke();
}