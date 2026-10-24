struct AutoTinyThunkTarget
// Leaf: CEntry>_const&>
{
    void Invoke();
};
extern AutoTinyThunkTarget g_AutoTinyThunkObject_013b8b70;
void __fastcall GlobalMethodTailThunk_01228f4d()
{
    g_AutoTinyThunkObject_013b8b70.Invoke();
}