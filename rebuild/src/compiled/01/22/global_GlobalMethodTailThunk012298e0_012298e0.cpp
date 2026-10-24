struct AutoTinyThunkTarget
// Leaf: CEntry>_const&>
{
    void Invoke();
};
extern AutoTinyThunkTarget g_AutoTinyThunkObject_013bacd4;
void __fastcall GlobalMethodTailThunk_012298e0()
{
    g_AutoTinyThunkObject_013bacd4.Invoke();
}