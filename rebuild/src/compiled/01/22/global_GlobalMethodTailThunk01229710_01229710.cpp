struct AutoTinyThunkTarget
// Leaf: CEntry>_const&>
{
    void Invoke();
};
extern AutoTinyThunkTarget g_AutoTinyThunkObject_013bac60;
void __fastcall GlobalMethodTailThunk_01229710()
{
    g_AutoTinyThunkObject_013bac60.Invoke();
}