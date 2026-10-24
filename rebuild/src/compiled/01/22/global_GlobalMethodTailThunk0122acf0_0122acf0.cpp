struct AutoTinyThunkTarget
// Leaf: CEntry>_const&>
{
    void Invoke();
};
extern AutoTinyThunkTarget g_AutoTinyThunkObject_01436ecc;
void __fastcall GlobalMethodTailThunk_0122acf0()
{
    g_AutoTinyThunkObject_01436ecc.Invoke();
}