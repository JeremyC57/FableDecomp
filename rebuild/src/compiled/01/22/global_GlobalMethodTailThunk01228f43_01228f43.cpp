struct AutoTinyThunkTarget
// Leaf: CEntry>_const&>
{
    void Invoke();
};
extern AutoTinyThunkTarget g_AutoTinyThunkObject_013b8b6c;
void __fastcall GlobalMethodTailThunk_01228f43()
{
    g_AutoTinyThunkObject_013b8b6c.Invoke();
}