struct AutoTinyThunkTarget
// Leaf: CEntry>_const&>
{
    void Invoke();
};
extern AutoTinyThunkTarget g_AutoTinyThunkObject_013bab0c;
void __fastcall GlobalMethodTailThunk_012293f0()
{
    g_AutoTinyThunkObject_013bab0c.Invoke();
}