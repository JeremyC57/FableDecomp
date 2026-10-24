struct AutoTinyThunkTarget
// Leaf: CEntry>_const&>
{
    void Invoke();
};
extern AutoTinyThunkTarget g_AutoTinyThunkObject_013b874c;
void __fastcall GlobalMethodTailThunk_01228abf()
{
    g_AutoTinyThunkObject_013b874c.Invoke();
}