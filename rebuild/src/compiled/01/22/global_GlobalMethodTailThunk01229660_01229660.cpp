struct AutoTinyThunkTarget
// Leaf: CEntry>_const&>
{
    void Invoke();
};
extern AutoTinyThunkTarget g_AutoTinyThunkObject_013bac34;
void __fastcall GlobalMethodTailThunk_01229660()
{
    g_AutoTinyThunkObject_013bac34.Invoke();
}