struct AutoTinyThunkTarget
// Leaf: CEntry>_const&>
{
    void Invoke();
};
extern AutoTinyThunkTarget g_AutoTinyThunkObject_013bacec;
void __fastcall GlobalMethodTailThunk_01229940()
{
    g_AutoTinyThunkObject_013bacec.Invoke();
}