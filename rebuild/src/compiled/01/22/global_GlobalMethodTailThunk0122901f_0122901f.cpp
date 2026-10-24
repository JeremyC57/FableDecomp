struct AutoTinyThunkTarget
// Leaf: CEntry>_const&>
{
    void Invoke();
};
extern AutoTinyThunkTarget g_AutoTinyThunkObject_013b8bc4;
void __fastcall GlobalMethodTailThunk_0122901f()
{
    g_AutoTinyThunkObject_013b8bc4.Invoke();
}