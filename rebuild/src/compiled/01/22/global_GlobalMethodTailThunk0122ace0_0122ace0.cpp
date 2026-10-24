struct AutoTinyThunkTarget
// Leaf: CEntry>_const&>
{
    void Invoke();
};
extern AutoTinyThunkTarget g_AutoTinyThunkObject_01436ec8;
void __fastcall GlobalMethodTailThunk_0122ace0()
{
    g_AutoTinyThunkObject_01436ec8.Invoke();
}