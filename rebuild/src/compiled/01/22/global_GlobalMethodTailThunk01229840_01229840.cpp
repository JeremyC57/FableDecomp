struct AutoTinyThunkTarget
// Leaf: CEntry>_const&>
{
    void Invoke();
};
extern AutoTinyThunkTarget g_AutoTinyThunkObject_013bacac;
void __fastcall GlobalMethodTailThunk_01229840()
{
    g_AutoTinyThunkObject_013bacac.Invoke();
}