struct AutoTinyThunkTarget
// Leaf: CEntry>_const&>
{
    void Invoke();
};
extern AutoTinyThunkTarget g_AutoTinyThunkObject_013bad90;
void __fastcall GlobalMethodTailThunk_01229a90()
{
    g_AutoTinyThunkObject_013bad90.Invoke();
}