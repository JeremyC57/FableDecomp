struct AutoTinyThunkTarget
// Leaf: CEntry>_const&>
{
    void Invoke();
};
extern AutoTinyThunkTarget g_AutoTinyThunkObject_013bacd8;
void __fastcall GlobalMethodTailThunk_012298f0()
{
    g_AutoTinyThunkObject_013bacd8.Invoke();
}