struct AutoTinyThunkTarget
// Leaf: CEntry>_const&>
{
    void Invoke();
};
extern AutoTinyThunkTarget g_AutoTinyThunkObject_013b8744;
void __fastcall GlobalMethodTailThunk_01228aab()
{
    g_AutoTinyThunkObject_013b8744.Invoke();
}