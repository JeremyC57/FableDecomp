struct AutoTinyThunkTarget
// Leaf: CEntry>_const&>
{
    void Invoke();
};
extern AutoTinyThunkTarget g_AutoTinyThunkObject_013b8bb4;
void __fastcall GlobalMethodTailThunk_01228ff7()
{
    g_AutoTinyThunkObject_013b8bb4.Invoke();
}