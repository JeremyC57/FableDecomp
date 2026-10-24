struct AutoTinyThunkTarget
// Leaf: CEntry>_const&>
{
    void Invoke();
};
extern AutoTinyThunkTarget g_AutoTinyThunkObject_013baccc;
void __fastcall GlobalMethodTailThunk_012298c0()
{
    g_AutoTinyThunkObject_013baccc.Invoke();
}