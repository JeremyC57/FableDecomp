struct AutoTinyThunkTarget
// Leaf: CEntry>_const&>
{
    void Invoke();
};
extern AutoTinyThunkTarget g_AutoTinyThunkObject_013bad68;
void __fastcall GlobalMethodTailThunk_01229a70()
{
    g_AutoTinyThunkObject_013bad68.Invoke();
}