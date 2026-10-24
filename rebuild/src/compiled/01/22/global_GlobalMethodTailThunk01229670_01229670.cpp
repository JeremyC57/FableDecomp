struct AutoTinyThunkTarget
// Leaf: CEntry>_const&>
{
    void Invoke();
};
extern AutoTinyThunkTarget g_AutoTinyThunkObject_013bac38;
void __fastcall GlobalMethodTailThunk_01229670()
{
    g_AutoTinyThunkObject_013bac38.Invoke();
}