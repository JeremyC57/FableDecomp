struct AutoTinyThunkTarget
// Leaf: CEntry>_const&>
{
    void Invoke();
};
extern AutoTinyThunkTarget g_AutoTinyThunkObject_013b8ba0;
void __fastcall GlobalMethodTailThunk_01228fc5()
{
    g_AutoTinyThunkObject_013b8ba0.Invoke();
}