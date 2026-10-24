struct AutoTinyThunkTarget
// Leaf: CEntry>_const&>
{
    void Invoke();
};
extern AutoTinyThunkTarget g_AutoTinyThunkObject_013baf28;
void __fastcall GlobalMethodTailThunk_01229f20()
{
    g_AutoTinyThunkObject_013baf28.Invoke();
}