struct AutoTinyThunkTarget
// Leaf: CEntry>_const&>
{
    void Invoke();
};
extern AutoTinyThunkTarget g_AutoTinyThunkObject_013ba894;
void __fastcall GlobalMethodTailThunk_01229240()
{
    g_AutoTinyThunkObject_013ba894.Invoke();
}