struct AutoTinyThunkTarget
// Leaf: CEntry>_const&>
{
    void Invoke();
};
extern AutoTinyThunkTarget g_AutoTinyThunkObject_013bacc8;
void __fastcall GlobalMethodTailThunk_012298b0()
{
    g_AutoTinyThunkObject_013bacc8.Invoke();
}