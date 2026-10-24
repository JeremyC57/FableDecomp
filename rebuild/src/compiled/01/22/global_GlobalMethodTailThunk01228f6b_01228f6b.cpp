struct AutoTinyThunkTarget
// Leaf: CEntry>_const&>
{
    void Invoke();
};
extern AutoTinyThunkTarget g_AutoTinyThunkObject_013b8b7c;
void __fastcall GlobalMethodTailThunk_01228f6b()
{
    g_AutoTinyThunkObject_013b8b7c.Invoke();
}