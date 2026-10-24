struct AutoTinyThunkTarget
// Leaf: CEntry>_const&>
{
    void Invoke();
};
extern AutoTinyThunkTarget g_AutoTinyThunkObject_013bac88;
void __fastcall GlobalMethodTailThunk_012297b0()
{
    g_AutoTinyThunkObject_013bac88.Invoke();
}