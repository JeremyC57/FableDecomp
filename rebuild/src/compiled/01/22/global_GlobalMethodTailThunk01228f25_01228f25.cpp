struct AutoTinyThunkTarget
// Leaf: CEntry>_const&>
{
    void Invoke();
};
extern AutoTinyThunkTarget g_AutoTinyThunkObject_013b8b60;
void __fastcall GlobalMethodTailThunk_01228f25()
{
    g_AutoTinyThunkObject_013b8b60.Invoke();
}