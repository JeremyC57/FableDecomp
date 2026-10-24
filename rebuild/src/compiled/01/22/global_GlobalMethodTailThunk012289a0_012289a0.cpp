struct AutoTinyThunkTarget
// Leaf: CEntry>_const&>
{
    void Invoke();
};
extern AutoTinyThunkTarget g_AutoTinyThunkObject_013b8674;
void __fastcall GlobalMethodTailThunk_012289a0()
{
    g_AutoTinyThunkObject_013b8674.Invoke();
}