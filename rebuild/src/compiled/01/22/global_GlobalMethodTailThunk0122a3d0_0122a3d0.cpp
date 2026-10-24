struct AutoTinyThunkTarget
// Leaf: CEntry>_const&>
{
    void Invoke();
};
extern AutoTinyThunkTarget g_AutoTinyThunkObject_013bb704;
void __fastcall GlobalMethodTailThunk_0122a3d0()
{
    g_AutoTinyThunkObject_013bb704.Invoke();
}