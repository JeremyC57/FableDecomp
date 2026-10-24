struct AutoTinyThunkTarget
// Leaf: CEntry>_const&>
{
    void Invoke();
};
extern AutoTinyThunkTarget g_AutoTinyThunkObject_013b8bb0;
void __fastcall GlobalMethodTailThunk_01228fed()
{
    g_AutoTinyThunkObject_013b8bb0.Invoke();
}