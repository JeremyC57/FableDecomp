struct AutoTinyThunkTarget
// Leaf: CEntry>_const&>
{
    void Invoke();
};
extern AutoTinyThunkTarget g_AutoTinyThunkObject_013b8b74;
void __fastcall GlobalMethodTailThunk_01228f57()
{
    g_AutoTinyThunkObject_013b8b74.Invoke();
}