struct AutoTinyThunkTarget
// Leaf: CEntry>_const&>
{
    void Invoke();
};
extern AutoTinyThunkTarget g_AutoTinyThunkObject_013badf8;
void __fastcall GlobalMethodTailThunk_01229b94()
{
    g_AutoTinyThunkObject_013badf8.Invoke();
}