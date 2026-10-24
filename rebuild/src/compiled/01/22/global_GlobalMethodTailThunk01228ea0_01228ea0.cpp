struct AutoTinyThunkTarget
// Leaf: CEntry>_const&>
{
    void Invoke();
};
extern AutoTinyThunkTarget g_AutoTinyThunkObject_013b8af4;
void __fastcall GlobalMethodTailThunk_01228ea0()
{
    g_AutoTinyThunkObject_013b8af4.Invoke();
}