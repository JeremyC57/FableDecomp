struct AutoTinyThunkTarget
// Leaf: CEntry>_const&>
{
    void Invoke();
};
extern AutoTinyThunkTarget g_AutoTinyThunkObject_013b8740;
void __fastcall GlobalMethodTailThunk_01228aa1()
{
    g_AutoTinyThunkObject_013b8740.Invoke();
}