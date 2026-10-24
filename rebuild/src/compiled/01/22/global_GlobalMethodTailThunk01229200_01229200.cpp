struct AutoTinyThunkTarget
// Leaf: CEntry>_const&>
{
    void Invoke();
};
extern AutoTinyThunkTarget g_AutoTinyThunkObject_013ba884;
void __fastcall GlobalMethodTailThunk_01229200()
{
    g_AutoTinyThunkObject_013ba884.Invoke();
}