struct AutoTinyThunkTarget
// Leaf: CEntry>_const&>
{
    void Invoke();
};
extern AutoTinyThunkTarget g_AutoTinyThunkObject_013bb67c;
void __fastcall GlobalMethodTailThunk_0122a290()
{
    g_AutoTinyThunkObject_013bb67c.Invoke();
}