struct AutoTinyThunkTarget
// Leaf: CEntry>_const&>
{
    void Invoke();
};
extern AutoTinyThunkTarget g_AutoTinyThunkObject_013b8b8c;
void __fastcall GlobalMethodTailThunk_01228f93()
{
    g_AutoTinyThunkObject_013b8b8c.Invoke();
}