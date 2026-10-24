struct AutoTinyThunkTarget
// Leaf: CEntry>_const&>
{
    void Invoke();
};
extern AutoTinyThunkTarget g_AutoTinyThunkObject_013bada4;
void __fastcall GlobalMethodTailThunk_01229af0()
{
    g_AutoTinyThunkObject_013bada4.Invoke();
}