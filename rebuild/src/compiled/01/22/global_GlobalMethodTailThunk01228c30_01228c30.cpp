struct AutoTinyThunkTarget
// Leaf: CEntry>_const&>
{
    void Invoke();
};
extern AutoTinyThunkTarget g_AutoTinyThunkObject_013b89d4;
void __fastcall GlobalMethodTailThunk_01228c30()
{
    g_AutoTinyThunkObject_013b89d4.Invoke();
}