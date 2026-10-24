struct AutoTinyThunkTarget
// Leaf: CEntry>_const&>
{
    void Invoke();
};
extern AutoTinyThunkTarget g_AutoTinyThunkObject_013bb680;
void __fastcall GlobalMethodTailThunk_0122a2a0()
{
    g_AutoTinyThunkObject_013bb680.Invoke();
}