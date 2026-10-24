struct AutoTinyThunkTarget
// Leaf: CEntry>_const&>
{
    void Invoke();
};
extern AutoTinyThunkTarget g_AutoTinyThunkObject_013bb6c8;
void __fastcall GlobalMethodTailThunk_0122a320()
{
    g_AutoTinyThunkObject_013bb6c8.Invoke();
}