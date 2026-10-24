struct AutoTinyThunkTarget
// Leaf: CEntry>_const&>
{
    void Invoke();
};
extern AutoTinyThunkTarget g_AutoTinyThunkObject_013bb740;
void __fastcall GlobalMethodTailThunk_0122a450()
{
    g_AutoTinyThunkObject_013bb740.Invoke();
}