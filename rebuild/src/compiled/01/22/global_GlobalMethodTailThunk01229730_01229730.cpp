struct AutoTinyThunkTarget
// Leaf: CEntry>_const&>
{
    void Invoke();
};
extern AutoTinyThunkTarget g_AutoTinyThunkObject_013bac68;
void __fastcall GlobalMethodTailThunk_01229730()
{
    g_AutoTinyThunkObject_013bac68.Invoke();
}