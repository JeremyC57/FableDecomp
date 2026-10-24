struct AutoTinyThunkTarget
// Leaf: CEntry>_const&>
{
    void Invoke();
};
extern AutoTinyThunkTarget g_AutoTinyThunkObject_013baca0;
void __fastcall GlobalMethodTailThunk_01229810()
{
    g_AutoTinyThunkObject_013baca0.Invoke();
}