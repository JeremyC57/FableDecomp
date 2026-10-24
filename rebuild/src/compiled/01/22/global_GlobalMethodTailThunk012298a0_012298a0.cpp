struct AutoTinyThunkTarget
// Leaf: CEntry>_const&>
{
    void Invoke();
};
extern AutoTinyThunkTarget g_AutoTinyThunkObject_013bacc4;
void __fastcall GlobalMethodTailThunk_012298a0()
{
    g_AutoTinyThunkObject_013bacc4.Invoke();
}