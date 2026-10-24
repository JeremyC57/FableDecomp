struct AutoTinyThunkTarget
// Leaf: CEntry>_const&>
{
    void Invoke();
};
extern AutoTinyThunkTarget g_AutoTinyThunkObject_013bade8;
void __fastcall GlobalMethodTailThunk_01229b60()
{
    g_AutoTinyThunkObject_013bade8.Invoke();
}