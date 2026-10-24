struct AutoTinyThunkTarget
// Leaf: CEntry>_const&>
{
    void Invoke();
};
extern AutoTinyThunkTarget g_AutoTinyThunkObject_013bab80;
void __fastcall GlobalMethodTailThunk_012294c0()
{
    g_AutoTinyThunkObject_013bab80.Invoke();
}