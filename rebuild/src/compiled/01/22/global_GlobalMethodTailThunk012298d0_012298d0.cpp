struct AutoTinyThunkTarget
// Leaf: CEntry>_const&>
{
    void Invoke();
};
extern AutoTinyThunkTarget g_AutoTinyThunkObject_013bacd0;
void __fastcall GlobalMethodTailThunk_012298d0()
{
    g_AutoTinyThunkObject_013bacd0.Invoke();
}