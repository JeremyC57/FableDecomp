struct AutoTinyThunkTarget
// Leaf: CEntry>_const&>
{
    void Invoke();
};
extern AutoTinyThunkTarget g_AutoTinyThunkObject_013bb670;
void __fastcall GlobalMethodTailThunk_0122a260()
{
    g_AutoTinyThunkObject_013bb670.Invoke();
}