struct AutoTinyThunkTarget
// Leaf: CEntry>_const&>
{
    void Invoke();
};
extern AutoTinyThunkTarget g_AutoTinyThunkObject_013bb6e0;
void __fastcall GlobalMethodTailThunk_0122a380()
{
    g_AutoTinyThunkObject_013bb6e0.Invoke();
}