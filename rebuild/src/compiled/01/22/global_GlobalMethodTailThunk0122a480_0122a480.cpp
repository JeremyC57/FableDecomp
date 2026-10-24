struct AutoTinyThunkTarget
// Leaf: CEntry>_const&>
{
    void Invoke();
};
extern AutoTinyThunkTarget g_AutoTinyThunkObject_013bc3a0;
void __fastcall GlobalMethodTailThunk_0122a480()
{
    g_AutoTinyThunkObject_013bc3a0.Invoke();
}