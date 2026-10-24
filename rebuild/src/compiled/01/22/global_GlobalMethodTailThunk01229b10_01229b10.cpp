struct AutoTinyThunkTarget
// Leaf: CEntry>_const&>
{
    void Invoke();
};
extern AutoTinyThunkTarget g_AutoTinyThunkObject_013badac;
void __fastcall GlobalMethodTailThunk_01229b10()
{
    g_AutoTinyThunkObject_013badac.Invoke();
}