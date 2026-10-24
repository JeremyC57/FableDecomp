struct AutoTinyThunkTarget
// Leaf: CEntry>_const&>
{
    void Invoke();
};
extern AutoTinyThunkTarget g_AutoTinyThunkObject_013b8b9c;
void __fastcall GlobalMethodTailThunk_01228fbb()
{
    g_AutoTinyThunkObject_013b8b9c.Invoke();
}