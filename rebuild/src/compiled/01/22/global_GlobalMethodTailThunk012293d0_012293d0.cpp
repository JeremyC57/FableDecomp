struct AutoTinyThunkTarget
// Leaf: CEntry>_const&>
{
    void Invoke();
};
extern AutoTinyThunkTarget g_AutoTinyThunkObject_013bab04;
void __fastcall GlobalMethodTailThunk_012293d0()
{
    g_AutoTinyThunkObject_013bab04.Invoke();
}