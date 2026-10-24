struct AutoTinyThunkTarget
// Leaf: CEntry>_const&>
{
    void Invoke();
};
extern AutoTinyThunkTarget g_AutoTinyThunkObject_013b8bc0;
void __fastcall GlobalMethodTailThunk_01229015()
{
    g_AutoTinyThunkObject_013b8bc0.Invoke();
}