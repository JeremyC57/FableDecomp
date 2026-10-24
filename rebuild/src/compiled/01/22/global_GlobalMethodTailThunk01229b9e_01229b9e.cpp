struct AutoTinyThunkTarget
// Leaf: CEntry>_const&>
{
    void Invoke();
};
extern AutoTinyThunkTarget g_AutoTinyThunkObject_013badfc;
void __fastcall GlobalMethodTailThunk_01229b9e()
{
    g_AutoTinyThunkObject_013badfc.Invoke();
}