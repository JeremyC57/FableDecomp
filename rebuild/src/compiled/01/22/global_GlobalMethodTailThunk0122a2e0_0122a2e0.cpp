struct AutoTinyThunkTarget
// Leaf: CEntry>_const&>
{
    void Invoke();
};
extern AutoTinyThunkTarget g_AutoTinyThunkObject_013bb694;
void __fastcall GlobalMethodTailThunk_0122a2e0()
{
    g_AutoTinyThunkObject_013bb694.Invoke();
}