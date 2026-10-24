struct AutoTinyThunkTarget
// Leaf: CEntry>_const&>
{
    void Invoke();
};
extern AutoTinyThunkTarget g_AutoTinyThunkObject_013b86fc;
void __fastcall GlobalMethodTailThunk_01228a10()
{
    g_AutoTinyThunkObject_013b86fc.Invoke();
}