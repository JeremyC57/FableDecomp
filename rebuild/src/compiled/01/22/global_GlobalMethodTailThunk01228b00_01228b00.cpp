struct AutoTinyThunkTarget
// Leaf: CEntry>_const&>
{
    void Invoke();
};
extern AutoTinyThunkTarget g_AutoTinyThunkObject_013b8778;
void __fastcall GlobalMethodTailThunk_01228b00()
{
    g_AutoTinyThunkObject_013b8778.Invoke();
}