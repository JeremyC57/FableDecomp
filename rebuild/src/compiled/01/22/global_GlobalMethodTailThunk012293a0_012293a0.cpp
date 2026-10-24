struct AutoTinyThunkTarget
// Leaf: CEntry>_const&>
{
    void Invoke();
};
extern AutoTinyThunkTarget g_AutoTinyThunkObject_013baaf8;
void __fastcall GlobalMethodTailThunk_012293a0()
{
    g_AutoTinyThunkObject_013baaf8.Invoke();
}