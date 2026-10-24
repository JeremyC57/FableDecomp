struct AutoTinyThunkTarget
// Leaf: CEntry>_const&>
{
    void Invoke();
};
extern AutoTinyThunkTarget g_AutoTinyThunkObject_013b8b78;
void __fastcall GlobalMethodTailThunk_01228f61()
{
    g_AutoTinyThunkObject_013b8b78.Invoke();
}