struct AutoTinyThunkTarget
// Leaf: CEntry>_const&>
{
    void Invoke();
};
extern AutoTinyThunkTarget g_AutoTinyThunkObject_013baf40;
void __fastcall GlobalMethodTailThunk_01229fb0()
{
    g_AutoTinyThunkObject_013baf40.Invoke();
}