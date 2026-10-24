struct AutoTinyThunkTarget
// Leaf: CEntry>_const&>
{
    void Invoke();
};
extern AutoTinyThunkTarget g_AutoTinyThunkObject_013b8ae8;
void __fastcall GlobalMethodTailThunk_01228e70()
{
    g_AutoTinyThunkObject_013b8ae8.Invoke();
}