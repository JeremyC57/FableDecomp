struct AutoTinyThunkTarget
// Leaf: CEntry>_const&>
{
    void Invoke();
};
extern AutoTinyThunkTarget g_AutoTinyThunkObject_013bc3a8;
void __fastcall GlobalMethodTailThunk_0122a490()
{
    g_AutoTinyThunkObject_013bc3a8.Invoke();
}