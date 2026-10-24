struct AutoTinyThunkTarget
// Leaf: CEntry>_const&>
{
    void Invoke();
};
extern AutoTinyThunkTarget g_AutoTinyThunkObject_013b8670;
void __fastcall GlobalMethodTailThunk_01228990()
{
    g_AutoTinyThunkObject_013b8670.Invoke();
}