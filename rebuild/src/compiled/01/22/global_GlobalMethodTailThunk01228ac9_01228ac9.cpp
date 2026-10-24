struct AutoTinyThunkTarget
// Leaf: CEntry>_const&>
{
    void Invoke();
};
extern AutoTinyThunkTarget g_AutoTinyThunkObject_013b8750;
void __fastcall GlobalMethodTailThunk_01228ac9()
{
    g_AutoTinyThunkObject_013b8750.Invoke();
}